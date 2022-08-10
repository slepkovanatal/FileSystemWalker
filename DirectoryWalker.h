#ifndef FILESYSTEMROUND_DIRECTORYWALKER_H
#define FILESYSTEMROUND_DIRECTORYWALKER_H

#include "ThreadPool.h"
#include "Consumer.h"

#include <filesystem>
#include <iostream>


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
                std::current_exception();
            }

            std::lock_guard<std::mutex> lock(m);
            ++completed_tasks;
            if (visited_fs_objects == processed_dirs &&
                    files_count == completed_tasks) {
                promise.set_value();
            }
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

    std::lock_guard<std::mutex> lock(m);
    ++processed_dirs;

    if (visited_fs_objects == processed_dirs &&
        files_count == completed_tasks) {
        promise.set_value();
    }
}


template<typename T>
std::future<void> DirectoryWalker<T>::getResults() {
    return promise.get_future();
}

#endif //FILESYSTEMROUND_DIRECTORYWALKER_H
