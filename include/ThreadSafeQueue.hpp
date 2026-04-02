#pragma once

#include <queue>
#include <mutex>
#include <optional>
#include <condition_variable>

template <typename T>
class IReadOnlyQueue {
public:
    virtual ~IReadOnlyQueue() = default;
    virtual std::optional<T> pop() = 0;
};

template <typename T>
class IWriteOnlyQueue {
public:
    virtual ~IWriteOnlyQueue() = default;
    virtual void push(const T& value) = 0;
    virtual void close() = 0;
};

template <typename T>
class ThreadSafeQueue: public IReadOnlyQueue<T>, public IWriteOnlyQueue<T> {
private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable value_available_;
    bool closed_ = false;

public:
    void push(const T& value) override {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(value));
        value_available_.notify_one(); 
    }

    std::optional<T> pop() override {
        std::unique_lock<std::mutex> lock(mutex_);
        value_available_.wait(lock, [this] { 
            return !queue_.empty() || closed_; 
        });
        if (queue_.empty() && closed_) {
            return std::nullopt;
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    void close() override {
        { // create a subscope here so that lock_guard is released before we notify reading threads to prevent wait-wake-block
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        value_available_.notify_all();
    }
};