#include "ThreadPool.h"
#include "DirectoryWalker.h"
#include "FileSearcher.h"

#include <iostream>
#include <filesystem>
#include <cassert>
#include <queue>


void roundDirectory(std::filesystem::path const &dir)
{
    for (auto const &it: std::filesystem::directory_iterator{dir}) {
        std::cout << it.path() << "\n";
    }
}

std::filesystem::path getChild(char *str_path)
{
    std::filesystem::path path(str_path);
    return begin(std::filesystem::directory_iterator{path})->path();
}


int main(int argc, char *argv[]) {
    assert(1 < argc);
    std::filesystem::path cur_dir = argv[1];

    DirectoryWalker round(cur_dir, [](std::filesystem::path file) {
        try {
            FileSearcher fileSearcher(file);
            if (fileSearcher.search("v -0.609689 -0.067739 0.098931")) {
                std::cout << "SUCCESS\n";
            }
        } catch (const std::exception &e) {
            std::cout << e.what() << "\n";
        }
    });
    return 0;
}
