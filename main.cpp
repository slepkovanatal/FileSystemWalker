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


int main(int argc, char *argv[]) {
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
    return 0;
}
