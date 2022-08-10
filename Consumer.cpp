#include "Consumer.h"


Consumer::Consumer():
    results()
{
}


std::vector<std::filesystem::path> Consumer::getResults()
{
    std::lock_guard<std::mutex> lock(m);
    return results;
}


void Consumer::addPath(const std::filesystem::path &path)
{
    std::lock_guard<std::mutex> lock(m);
    results.push_back(path);
}
