#ifndef FILESYSTEMROUND_FILESYSTEMSEARCHER_H
#define FILESYSTEMROUND_FILESYSTEMSEARCHER_H

#include "Consumer.h"
#include "DirectoryWalker.h"

#include <future>
#include <filesystem>


class FileSystemSearcher
{
public:
    explicit FileSystemSearcher(const std::filesystem::path &path,
                                Consumer &consumer);

    std::future<void> doSearch();

private:
    DirectoryWalker<bool> walker;
};

#endif //FILESYSTEMROUND_FILESYSTEMSEARCHER_H
