#ifndef FILESYSTEMROUND_DIRECTORYWALKER_H
#define FILESYSTEMROUND_DIRECTORYWALKER_H

#include "ThreadPool.h"

#include <filesystem>
#include <iostream>


template<typename T>
class DirectoryWalker
{
public:
    explicit DirectoryWalker(
            std::filesystem::path dir,
            std::function<void(std::promise<T>&&, std::filesystem::path)> func);

    std::vector<std::pair<std::filesystem::path, T>> getResults();

    ~DirectoryWalker();

private:
    void walk(std::filesystem::path dir);

    void end_process();

private:
    std::filesystem::path start_dir;
    ThreadPool pool;
    std::vector<std::future<void>> walk_futures;
    std::mutex m;
    std::atomic<int> processed_dirs;
    std::condition_variable cv;
    std::function<void(std::promise<T> &&, std::filesystem::path)> func;
    std::vector<std::thread> threads;
    std::vector<std::pair<std::filesystem::path, std::future<T>>> fun_futures;
};


template<typename T>
DirectoryWalker<T>::DirectoryWalker(std::filesystem::path dir,
                                    std::function<void(std::promise<T> &&, std::filesystem::path)> func):
        start_dir(std::move(dir)),
        pool(16),
        processed_dirs(0),
        threads(),
        func(func)
{
}


template<typename T>
void DirectoryWalker<T>::walk(std::filesystem::path dir) {
    {
        std::lock_guard<std::mutex> lock(m);
        std::promise<T> promise;
        fun_futures.push_back({dir, promise.get_future()});
        threads.emplace_back(func, std::move(promise), dir);
    }
    std::string path = dir.string();
//    std::cout << dir << "\n";
    if (std::filesystem::is_directory(dir)) {
        for (const auto &it: std::filesystem::directory_iterator{dir}) {
            std::filesystem::path cur_path = it.path();
            std::lock_guard<std::mutex> lock(m);
            walk_futures.push_back(pool.add([this, cur_path]() {
                this->walk(cur_path);
            }));
        }
    }
    {
        std::lock_guard<std::mutex> lock(m);
        ++processed_dirs;
    }
    cv.notify_all();
}


template<typename T>
DirectoryWalker<T>::~DirectoryWalker()
{
    end_process();
}


template<typename T>
std::vector<std::pair<std::filesystem::path, T>> DirectoryWalker<T>::getResults() {
    {
        std::lock_guard<std::mutex> lock(m);
        walk_futures.push_back(pool.add([this]() { this->walk(start_dir); }));
    }
    end_process();
    std::vector<std::pair<std::filesystem::path, T>> res;
    {
        std::lock_guard<std::mutex> lock(m);
        std::transform(fun_futures.begin(), fun_futures.end(), std::back_inserter(res),
                       [](std::pair<std::filesystem::path, std::future<T>> &p) {
                            try {
                                return std::make_pair(p.first, p.second.get());
                            } catch (const std::exception &e) {
                                std::cout << e.what() << "\n";
                            }
                       });
    }
    return res;
}


template<typename T>
void DirectoryWalker<T>::end_process() {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]() -> bool {
        auto result = processed_dirs == walk_futures.size();
        return result;
    });

    for (auto &t: threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    lock.unlock();
}
#endif //FILESYSTEMROUND_DIRECTORYWALKER_H
