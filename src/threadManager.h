#pragma once
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <mutex>

class threadManager {
    struct ThreadData {
        std::function<void(size_t)> func;
        std::atomic<bool> hasWork{false};
        std::atomic<bool> stop{false};
        std::condition_variable cv;
        std::mutex mtx;
        size_t start = 0;
        size_t end = 0;
    };

    static inline std::vector<std::thread> workers;
    static inline std::vector<std::unique_ptr<ThreadData>> thread_data;
    static inline std::once_flag init_flag;

    static void worker_loop(ThreadData* data) {
        while (true) {
            std::unique_lock lock(data->mtx);
            data->cv.wait(lock, [&] { return data->hasWork || data->stop; });

            if (data->stop) break;

            for (size_t i = data->start; i < data->end; ++i) {
                data->func(i);
            }

            data->hasWork = false;
        }
    }

public:
    static void multithread_range(std::function<void(size_t)> func, size_t start, size_t end, size_t thread_count = std::thread::hardware_concurrency()) {
        std::call_once(init_flag, [&] {
            thread_count = std::max<size_t>(1, thread_count);
            thread_data.reserve(thread_count);
            workers.reserve(thread_count);
            for (size_t i = 0; i < thread_count; ++i) {
                auto data = std::make_unique<ThreadData>();
                workers.emplace_back(worker_loop, data.get());
                thread_data.push_back(std::move(data));
            }
        });

        thread_count = std::min(thread_count, end - start);
        if (thread_count <= 1) {
            for (size_t i = start; i < end; ++i) func(i);
            return;
        }

        const size_t chunk_size = (end - start + thread_count - 1) / thread_count;
        size_t current = start;

        std::vector<ThreadData*> used_threads;

        for (size_t i = 0; i < thread_count && current < end; ++i) {
            auto* data = thread_data[i].get();
            data->start = current;
            data->end = std::min(current + chunk_size, end);
            data->func = func;
            {
                std::lock_guard lock(data->mtx);
                data->hasWork = true;
            }
            data->cv.notify_one();
            used_threads.push_back(data);
            current = data->end;
        }

        for (auto* data : used_threads) {
            while (data->hasWork) {
                std::this_thread::yield();
            }
        }
    }

    static void shutdown() {
        for (auto& data : thread_data) {
            {
                std::lock_guard lock(data->mtx);
                data->stop = true;
                data->cv.notify_one();
            }
        }
        for (auto& t : workers) {
            if (t.joinable()) t.join();
        }
        workers.clear();
        thread_data.clear();
    }
};
