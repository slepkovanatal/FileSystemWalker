#ifndef FILESYSTEMROUND_FILESEARCHER_H
#define FILESYSTEMROUND_FILESEARCHER_H


#include <filesystem>

class FileSearcher
{
public:
    explicit FileSearcher(const std::filesystem::path &file);

    bool search(const std::string &s);

private:
    int polynomialHashCode(const std::string &s);

    int updateHash(int firstChar, int lastChar, int prev_hash);

private:
    const int m = 1e9 + 7;
    const int p = 31;

private:
    std::filesystem::path file;
    int p_pow;
};


#endif //FILESYSTEMROUND_FILESEARCHER_H
