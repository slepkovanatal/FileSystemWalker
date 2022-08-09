#ifndef FILESYSTEMROUND_THREADPOOL_H
#define FILESYSTEMROUND_THREADPOOL_H

#include <queue>
#include <future>
#include <thread>
#include <filesystem>


class ThreadPool
{
public:
    explicit ThreadPool(size_t threadCount);

    std::future<void> add(
                std::function<void()> task);

    ~ThreadPool();

private:
    void run();

    struct Task
    {
        std::function<void()> func;
        std::promise<void> promise;
    };

private:
    std::queue<Task> q;
    std::condition_variable q_cv;
    std::mutex m;
    std::vector<std::thread> threads;
    bool quite;
};


#endif //FILESYSTEMROUND_THREADPOOL_H
