#ifndef FILESYSTEMROUND_CONSUMER_H
#define FILESYSTEMROUND_CONSUMER_H

#include <vector>
#include <filesystem>
#include <mutex>


class Consumer {
public:
    explicit Consumer();

    void addPath(const std::filesystem::path &path);

    std::vector<std::filesystem::path> getResults();

private:
   std::vector<std::filesystem::path> results;
   std::mutex m;
};


#endif //FILESYSTEMROUND_CONSUMER_H
