#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class ThreadRunner
{
    public:

    static std::vector<int> v;
    static int counter;
    static std::mutex access_vector;
    

    static void f()
    {
        std::cout << std::this_thread::get_id() << " is running\n";
        access_vector.lock();
        v.push_back(counter);
        ++counter;
        access_vector.unlock();
    }

    void runThreads(int threads_count)
    {
        for (int i = 0; i < threads_count; ++i)
        {
            std::thread t(f);
            t.join();
        }
        for (auto value : v)
        {
            std::cout << value << '\t';
        }
        std::cout << "\n";
    }

};

std::vector<int> ThreadRunner::v = {};
int ThreadRunner::counter = 0;
std::mutex ThreadRunner::access_vector;