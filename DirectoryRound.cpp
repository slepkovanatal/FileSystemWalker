#include "DirectoryRound.h"

#include <thread>
#include <iostream>
#include <mutex>


DirectoryRound::DirectoryRound(const std::filesystem::path &dir):
    pool(1)
{
    std::lock_guard<std::mutex> lock(m);
    futures.push_back(pool.add([this](std::filesystem::path cur_dir){
        return this->round(cur_dir);}, dir));
}


void DirectoryRound::round(const std::filesystem::path &dir) {
    std::string path = dir.string();
    std::cout << dir << "\n";
    for (auto it: std::filesystem::directory_iterator{dir}) {
        std::string cur_path = it.path().string();
        std::lock_guard<std::mutex> lock(m);
        futures.push_back(pool.add([this, it.path()]() {
            return this->round(cur_dir);
        }));
    }
}

DirectoryRound::~DirectoryRound()
{
    m.lock();
    for (auto &future: futures) {
        m.unlock();
        try {
            future.get();
        } catch (const std::exception &e) {
            std::cout << e.what() << "\n";
        }
    }
}
