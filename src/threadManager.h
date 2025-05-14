#pragma once
#include <thread>
#include <functional>
#include <iostream>
#include <tgmath.h>
using namespace std;

class threadManager {
    inline static vector<thread> thread_pool{};
public:
    static void multithread_range(function<void(size_t)> func = nullptr, size_t start = 0, size_t end = 0, size_t thread_count = thread::hardware_concurrency() - 1) {
        thread_count = max(static_cast<size_t>(thread::hardware_concurrency()) - 1, thread_count);

        clear_threads();
        if (thread_pool.size() != thread_count) {
            thread_pool.reserve(thread_count);
        }

        float chunk_size = (end - start - 1) / static_cast<float>(thread_count);
        if (chunk_size < 2) {
            for (int i = 0; i < end; i++) {
                func(i);
            }
        } else {
            float s = start;
            float e = start + chunk_size;
            for (int i = 0; i < thread_count; i++) {
                size_t s_thread = ceil(s);
                size_t e_thread = min(ceil(e), static_cast<float>(end));
                // cout << s_thread << "-" << e_thread << " " << e_thread - s_thread << endl;
                thread_pool.emplace_back([&func, s_thread, e_thread]{for (int i = s_thread; i < e_thread; i++) func(i);});
                s += chunk_size;
                e += chunk_size;
            }
        }

        for (int i = 0; i < thread_count; i++) {
            if (thread_pool[i].joinable()) {thread_pool[i].join();}
        }
    }

    static void clear_threads() {
        for (auto& t : thread_pool) {
            if (t.joinable()) {
                t.join();
            }
        }
        thread_pool.clear();
    }
};
