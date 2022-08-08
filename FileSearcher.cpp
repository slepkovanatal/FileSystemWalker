#include "FileSearcher.h"

#include <queue>
#include <fstream>
#include <cassert>
#include <iostream>


FileSearcher::FileSearcher(const std::filesystem::path &file):
    file(file)
{
}


bool FileSearcher::search(const std::string &s) {
    const long long s_hash = polynomialHashCode(s);
    std::ifstream fileStream(file);
    char buffer[s.size()];
    fileStream.read(buffer, s.size());
    std::queue<char> q;
    for (char c: buffer) {
        q.push(c);
    }
    long long buf_hash = polynomialHashCode(std::string(buffer, s.size()));
    computePPow(s.size() - 1);
    while (buf_hash != s_hash && fileStream.peek() != EOF) {
        char buff[1];
        fileStream.read(buff, 1);
        q.push(buff[0]);
        buf_hash = updateHash(int(q.front()), int(q.back()), buf_hash);
        q.pop();
    }
    return buf_hash == s_hash;
}


long long FileSearcher::polynomialHashCode(const std::string &s) {
    long long hash = 0;
    for (char c: s) {
        hash = ((hash * p) % m + int(c)) % m;
    }
    return hash;
}


long long FileSearcher::updateHash(long long firstChar, long long lastChar, long long prev_hash) {
    prev_hash = (prev_hash - (firstChar * p_pow) % m + m) % m;
    prev_hash = ((prev_hash * p) % m + lastChar) % m;
    return prev_hash;
}


void FileSearcher::computePPow(int n) {
    p_pow = 1;
    for (size_t i = 0; i < n; ++i) {
        p_pow = (p_pow * p) % m;
    }
}
