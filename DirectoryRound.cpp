#include "DirectoryRound.h"

#include <thread>
#include <iostream>
#include <mutex>


DirectoryRound::DirectoryRound(const std::filesystem::path &dir):
    pool(5),
    processed_dirs(0)
{
    std::lock_guard<std::mutex> lock(m);
    futures.push_back(pool.add([this, dir](){ this->round(dir);}));
}


void DirectoryRound::round(std::filesystem::path dir) {
    try {
        std::cout << "begin\n";
        std::string path = dir.string();
        std::cout << dir << "\n";
        if (std::filesystem::is_directory(dir)) {
            for (const auto &it: std::filesystem::directory_iterator{dir}) {
                std::filesystem::path cur_path = it.path();
                std::lock_guard<std::mutex> lock(m);
                futures.push_back(pool.add([this, cur_path]() {
                    this->round(cur_path);
                }));
            }
        }
        std::cout << "end\n";
        ++processed_dirs;
    } catch (std::exception const &e) {
        std::cout << e.what() << '\n';
    }
}


DirectoryRound::~DirectoryRound()
{
    while (true) {
        std::lock_guard<std::mutex> lock(m);
        std::cout << processed_dirs << ' ' << futures.size() << "\n";
        if (processed_dirs == futures.size()) {
            break;
        }
    }
//    m.lock();
//    for (auto &future: futures) {
//        m.unlock();
//        try {
//            future.get();
//        } catch (const std::exception &e) {
//            std::cout << e.what() << "\n";
//        }
//    }
}
