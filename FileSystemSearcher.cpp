#include "FileSystemSearcher.h"
#include "DirectoryWalker.h"
#include "FileSearcher.h"


FileSystemSearcher::FileSystemSearcher(const std::filesystem::path &path, Consumer &consumer):
    walker(path, [&consumer](std::filesystem::path file) {
            FileSearcher fileSearcher(file);
            if (fileSearcher.search("v -0.609689 -0.067739 0.098931")) {
                consumer.addPath(file);
            }
        })
{
}


std::future<void> FileSystemSearcher::doSearch() {

    return walker.getResults();
}
