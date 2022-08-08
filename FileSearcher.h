#ifndef FILESYSTEMROUND_FILESEARCHER_H
#define FILESYSTEMROUND_FILESEARCHER_H


#include <filesystem>

class FileSearcher
{
public:
    explicit FileSearcher(const std::filesystem::path &file);

    bool search(const std::string &s);

private:
    long long polynomialHashCode(const std::string &s);

    long long updateHash(long long firstChar, long long lastChar, long long prev_hash);

    void computePPow(int n);

private:
    const long long m = 1e9 + 7;
    const long long p = 31;

private:
    std::filesystem::path file;
    long long p_pow;
};


#endif //FILESYSTEMROUND_FILESEARCHER_H
