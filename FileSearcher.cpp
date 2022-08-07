#include "FileSearcher.h"

#include <queue>
#include <fstream>
#include <cassert>


FileSearcher::FileSearcher(const std::filesystem::path &file):
    file(file)
{
}


bool FileSearcher::search(const std::string &s) {
    const int s_hash = polynomialHashCode(s);
    std::ifstream fileStream(file, std::ios::in);
    char buffer[s.size()];
    std::queue<char> q;
    for (char c: buffer) {
        q.push(c);
    }
    fileStream.read(buffer, sizeof(buffer));
    int buf_hash = polynomialHashCode(buffer);
    while (buf_hash != s_hash && fileStream.peek() != EOF) {
        char cur_char;
        fileStream >> cur_char;
        q.push(cur_char);
        buf_hash = updateHash(q.front(), q.back(), buf_hash);
        q.pop();
    }
    return buf_hash == s_hash;
}


int FileSearcher::polynomialHashCode(const std::string &s) {
    p_pow = 1;
    int hash = 0;
    for (char c: s) {
        hash = (hash + int(c) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}


int FileSearcher::updateHash(int firstChar, int lastChar, int prev_hash) {
    assert((prev_hash - firstChar) % p == 0);
    prev_hash = (prev_hash - firstChar) / p;
    prev_hash = (prev_hash + lastChar * p_pow) % m;
    return prev_hash;
}
