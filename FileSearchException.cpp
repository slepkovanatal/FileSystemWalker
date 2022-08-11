#include "FileSearchException.h"


FileSearchException::FileSearchException(
        std::vector<std::exception_ptr> exceptions):
    exceptions(std::move(exceptions))
{
    for (auto &eptr: this->exceptions) {
        try {
            std::rethrow_exception(eptr);
        } catch (const std::exception &e) {
            msg += std::string(e.what()) + "\n";
        }
    }
}


bool FileSearchException::empty()
{
    return exceptions.empty();
}


const char *FileSearchException::what() const noexcept
{
    return msg.c_str();
}
