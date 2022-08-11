#include "ThreadPool.h"
#include "FileSystemSearcher.h"

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
    for (int i = 0; i < 1000; i++) {
        assert(1 < argc);
        std::filesystem::path cur_dir = argv[1];

        Consumer consumer;
        FileSystemSearcher fs_searcher("C:\\Users\\Nata\\Documents\\scull_MultiView_test\\", consumer);
        std::future<void> future = fs_searcher.doSearch();

        try {
            future.get();
        } catch (std::exception &e) {
            std::cout << e.what() << "\n";
        }
        for (auto &file: consumer.getResults()) {
            std::cout << file << '\n';
        }
    }
    return 0;
}
