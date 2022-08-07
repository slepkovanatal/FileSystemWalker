cmake_minimum_required(VERSION 3.23)
project(FileSystemRound)

set(CMAKE_CXX_STANDARD 17)

add_executable(FileSystemRound main.cpp ThreadPool.cpp ThreadPool.h DirectoryRound.cpp DirectoryRound.h FileSearcher.cpp FileSearcher.h)