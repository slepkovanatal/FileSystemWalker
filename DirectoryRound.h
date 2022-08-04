#ifndef FILESYSTEMROUND_DIRECTORYROUND_H
#define FILESYSTEMROUND_DIRECTORYROUND_H

#include "ThreadPool.h"

#include <filesystem>


class DirectoryRound
{
public:
    explicit DirectoryRound(std::filesystem::path const &dir);

    void round(const std::filesystem::path &dir);

    ~DirectoryRound();

private:
    ThreadPool pool;
    std::vector<std::future<void>> futures;
    std::mutex m;
};

#endif //FILESYSTEMROUND_DIRECTORYROUND_H
