#include "ThreadPool.h"
#include "DirectoryRound.h"

#include <iostream>
#include <filesystem>
#include <cassert>
#include <queue>
#include <future>
#include <unistd.h>


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
//    for (int i = 0; i < 1000; i++) {
        assert(1 < argc);
        std::filesystem::path cur_dir = argv[1];
//    std::cout << "Current root name is: " << std::filesystem::current_path().root_name() << '\n';

//    ThreadPool pool(2);
//    size_t const tasks_count = 10;
//    std::vector<std::future<void>> futures(tasks_count);
//    for (size_t i = 0; i < tasks_count; ++i) {
//        futures[i] = pool.add([i](){ std::cout << i << ' '; });
//    }
//    for (auto &f: futures) {
//        f.wait();
//    }

        DirectoryRound round(cur_dir);
//    sleep(10000);
//    auto child = getChild(argv[1]);
//    for (const auto &it: std::filesystem::directory_iterator{child}) {
//        std::cout << it.path() << "\n";
//    }
//    std::cout << child.string();
//    }
    return 0;
}
