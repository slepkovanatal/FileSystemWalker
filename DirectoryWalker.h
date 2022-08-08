#ifndef FILESYSTEMROUND_DIRECTORYWALKER_H
#define FILESYSTEMROUND_DIRECTORYWALKER_H

#include "ThreadPool.h"

#include <filesystem>


class DirectoryWalker
{
public:
    explicit DirectoryWalker(
            std::filesystem::path const &dir,
            std::function<void(std::filesystem::path)> func);

    ~DirectoryWalker();

private:
    void walk(std::filesystem::path dir);

private:
    ThreadPool pool;
    std::vector<std::future<void>> futures;
    std::mutex m;
    std::atomic<int> processed_dirs;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::function<void(std::filesystem::path)> func;
};

#endif //FILESYSTEMROUND_DIRECTORYWALKER_H
