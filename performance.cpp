#include <thread>
#include <iostream>
#include "BlockQueue.hpp"

static const int MAX_ITERATION = 10000000;

void fun(TinyLib::BlockQueue<int>& queue)
{
    int value=0;
    auto start = std::chrono::steady_clock::now();
    for(int i=0;queue.pop(value)==0;i++)
    {
        if(i!=value){
            std::cout<<"bug found\n.";
            break;
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto time = std::chrono::duration<float>(end-start).count();
    std::cout<<"total:"<<value<<" time:"<<time<<std::endl;
    std::cout<<"performance:"<<value/time<<" iops\n";
}

template<typename T>
void queue_perf(T& q)
{
    auto start = std::chrono::steady_clock::now();

    for(int i=0;i<MAX_ITERATION;i++){
        q.push(i);
    }
    auto end = std::chrono::steady_clock::now();   
    
    std::cout<<"queue performance:"<<MAX_ITERATION/std::chrono::duration<float>(end-start).count()<<" iops\n";    
}

void mutex_perf()
{
    std::mutex mtx;
    auto start = std::chrono::steady_clock::now();

    for(int i=0;i<MAX_ITERATION;i++){
        std::unique_lock<std::mutex> lock(mtx);
    }
    auto end = std::chrono::steady_clock::now();   
    
    std::cout<<"mutex performance:"<<MAX_ITERATION/std::chrono::duration<float>(end-start).count()<<" iops\n";    
}
void block_queue_perf()
{
    TinyLib::BlockQueue<int> queue(100); //queue size will affect the result
    auto t=std::thread(fun,std::ref(queue));
    for(int i=0;i<MAX_ITERATION;i++){
        queue.push(i);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    queue.exit();
    t.join();    
}

int main()
{
    block_queue_perf();

    std::queue<int> q;
    queue_perf(q);

    mutex_perf();
    return 0;
}