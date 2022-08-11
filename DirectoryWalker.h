#ifndef FILESYSTEMROUND_DIRECTORYWALKER_H
#define FILESYSTEMROUND_DIRECTORYWALKER_H

#include "ThreadPool.h"
#include "Consumer.h"
#include "FileSearchException.h"

#include <filesystem>
#include <iostream>


class err : public std::exception
{
public:
    const char* what() const noexcept { return "error"; }
};


template<typename T>
class DirectoryWalker
{
public:
    explicit DirectoryWalker(
            std::filesystem::path dir,
            std::function<void(std::filesystem::path)> func);

    std::future<void> getResults();

private:
    void walk(std::filesystem::path dir);

    void checkEndCondition();

private:
    std::filesystem::path start_dir;
    ThreadPool pool;
    std::mutex m;
    int visited_fs_objects;
    std::atomic<int> processed_dirs;
    std::function<void(std::filesystem::path)> func;
    int completed_tasks;
    int files_count;
    std::promise<void> promise;
    std::vector<std::exception_ptr> exceptions;
};


template<typename T>
DirectoryWalker<T>::DirectoryWalker(std::filesystem::path dir,
                                    std::function<void(std::filesystem::path)> func):
        start_dir(std::move(dir)),
        pool(16),
        visited_fs_objects(0),
        processed_dirs(0),
        func(std::move(func)),
        completed_tasks(0),
        files_count(0)
{
    std::lock_guard<std::mutex> lock(m);

    ++visited_fs_objects;
    pool.add([this]() {
        this->walk(start_dir);
    });
}


template<typename T>
void DirectoryWalker<T>::walk(std::filesystem::path dir) {
    if (!std::filesystem::is_directory(dir)) {
        std::lock_guard<std::mutex> lock(m);
        ++files_count;
        pool.add([this, dir]() {
            try {
                func(dir);
            } catch (std::exception &e) {
                std::lock_guard<std::mutex> lock(m);
                exceptions.push_back(std::current_exception());
            }

            {
                std::lock_guard<std::mutex> lock(m);
                ++completed_tasks;
            }
            checkEndCondition();
        });
    }

    if (std::filesystem::is_directory(dir)) {
        for (const auto &it: std::filesystem::directory_iterator{dir}) {
            std::filesystem::path cur_path = it.path();
            std::lock_guard<std::mutex> lock(m);

            ++visited_fs_objects;
            pool.add([this, cur_path]() {
                this->walk(cur_path);
            });
        }
    }

    {
        std::lock_guard<std::mutex> lock(m);
        ++processed_dirs;
    }
    checkEndCondition();
}


template<typename T>
std::future<void> DirectoryWalker<T>::getResults() {
    return promise.get_future();
}


template<typename T>
void DirectoryWalker<T>::checkEndCondition() {
    std::lock_guard<std::mutex> lock(m);
    if (visited_fs_objects == processed_dirs &&
            files_count == completed_tasks) {
        if (exceptions.empty()) {
            promise.set_value();
        } else {
            promise.set_exception(std::make_exception_ptr(FileSearchException(exceptions)));
        }
    }
}

#endif //FILESYSTEMROUND_DIRECTORYWALKER_H
