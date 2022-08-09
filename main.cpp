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

    DirectoryWalker<bool> walker(cur_dir, [](std::promise<bool> &&p, std::filesystem::path file) {
        try {
            FileSearcher fileSearcher(file);
            p.set_value(fileSearcher.search("v -0.609689 -0.067739 0.098931"));
        } catch (const std::exception &e) {
            p.set_exception(std::make_exception_ptr(e));
        }
    });

    auto res = walker.getResults();
    for (auto &p: res) {
        if (p.second) {
            std::cout << p.first << '\n';
        }
    }
    return 0;
}
