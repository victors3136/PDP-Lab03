//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_SYNCHRONIZEDTASKQUEUE_HPP
#define LAB3_SIMPLY_PARALLEL_SYNCHRONIZEDTASKQUEUE_HPP

#include <optional>
#include <queue>
#include <mutex>
#include <functional>

using Task = std::function<void()>;

class SynchronizedTaskQueue {
private:
    std::queue<Task> data;
    std::mutex locker;
public:
    SynchronizedTaskQueue() : data{}, locker{} {};


    std::optional<Task> peek() {
        std::lock_guard<std::mutex> lockGuard{locker};
        if (data.empty()) { return {}; }
        return data.front();
    }

    void put(Task &&task) {
        std::lock_guard<std::mutex> lockGuard{locker};
        data.emplace(task);
    }

    std::optional<Task> pop() {
        std::lock_guard<std::mutex> lockGuard{locker};
        if (data.empty()) { return {}; }
        auto position = data.front();
        data.pop();
        return position;
    }
};


#endif //LAB3_SIMPLY_PARALLEL_SYNCHRONIZEDTASKQUEUE_HPP
