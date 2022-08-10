cmake_minimum_required(VERSION 3.23)
project(FileSystemRound)

set(CMAKE_CXX_STANDARD 17)

add_executable(FileSystemRound main.cpp ThreadPool.cpp ThreadPool.h DirectoryWalker.h FileSearcher.cpp FileSearcher.h Consumer.cpp Consumer.h FileSystemSearcher.cpp FileSystemSearcher.h)