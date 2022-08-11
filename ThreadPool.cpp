#include "ThreadPool.h"

#include <future>


ThreadPool::ThreadPool(size_t threadCount):
    quite(false)
{
    for (size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back(&ThreadPool::run, this);
    }
}


void ThreadPool::run() {
    while (true) {
        std::unique_lock<std::mutex> lock(m);
        q_cv.wait(lock, [this]() -> bool { return !this->q.empty() || quite; });

        if (quite) {
            return;
        }

        if (!q.empty()) {
            Task task = std::move(q.front());
            q.pop();
            lock.unlock();
            try {
                task.func();
                task.promise.set_value();
            } catch (std::exception const &e) {
                task.promise.set_exception(std::make_exception_ptr(e));
            }
        }
    }
}


std::future<void> ThreadPool::add(
        std::function<void()> task) {
    std::unique_lock<std::mutex> lock(m);
    std::promise<void> task_promise;
    std::future<void> task_future = task_promise.get_future();
    q.push({task, std::move(task_promise)});
    lock.unlock();
    q_cv.notify_one();
    return task_future;
}


ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(m);
        quite = true;
    }
    q_cv.notify_all();
    for (auto &t: threads) {
        t.join();
    }
}
