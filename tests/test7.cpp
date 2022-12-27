#include <cassert>
#include <thread>
#include "BlockQueue.hpp"

static const int MAX_ITERATION = 1000;

void fun(TinyLib::BlockQueue<int>& queue)
{
    int value;
    for(int i=0;queue.pop(value)==0;i++)
    {
        assert(i==value);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
int main()
{
    TinyLib::BlockQueue<int> queue(5); 
    auto t=std::thread(fun,std::ref(queue));
    for(int i=0;i<MAX_ITERATION;i++){
        assert(queue.push(i)==0);
    }
    queue.exit();
    t.join();
    return 0;
}