#ifndef FILESYSTEMROUND_FILESEARCHEXCEPTION_H
#define FILESYSTEMROUND_FILESEARCHEXCEPTION_H

#include <vector>
#include <exception>
#include <mutex>


class FileSearchException: public std::exception
{
public:
    explicit FileSearchException(
            std::vector<std::exception_ptr> exceptions);

    bool empty();

    const char *what() const noexcept override;

private:
    std::vector<std::exception_ptr> exceptions;
    std::string msg;
};


#endif //FILESYSTEMROUND_FILESEARCHEXCEPTION_H
