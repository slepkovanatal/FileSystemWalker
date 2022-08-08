#include "DirectoryWalker.h"

#include <thread>
#include <iostream>
#include <mutex>


DirectoryWalker::DirectoryWalker(
        const std::filesystem::path &dir,
        std::function<void(std::filesystem::path)> func):
    pool(16),
    processed_dirs(0),
    threads(),
    func(std::move(func))
{
    std::lock_guard<std::mutex> lock(m);
    futures.push_back(pool.add([this, dir](){ this->walk(dir);}));
}


void DirectoryWalker::walk(
        std::filesystem::path dir) {
    try {
        {
            std::lock_guard<std::mutex> lock(m);
            threads.emplace_back(func, dir);
        }
        std::cout << "begin\n";
        std::string path = dir.string();
        std::cout << dir << "\n";
        if (std::filesystem::is_directory(dir)) {
            for (const auto &it: std::filesystem::directory_iterator{dir}) {
                std::filesystem::path cur_path = it.path();
                std::lock_guard<std::mutex> lock(m);
                futures.push_back(pool.add([this, cur_path]() {
                    this->walk(cur_path);
                }));
            }
        }
        std::cout << "end\n";
        {
            std::lock_guard<std::mutex> lock(m);
            ++processed_dirs;
        }
        cv.notify_all();
    } catch (std::exception const &e) {
        std::cout << e.what() << '\n';
    }
}


DirectoryWalker::~DirectoryWalker()
{
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [this]() -> bool {
        auto result = processed_dirs == futures.size();
        std::cout << processed_dirs << ' ' << futures.size() << " " << result << " " << std::this_thread::get_id() << "\n";
        return result;
    });

    std::cout << "finish";

    for (auto &t: threads) {
        t.join();
    }
    lock.unlock();
}
