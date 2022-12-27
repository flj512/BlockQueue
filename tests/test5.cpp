#include <cassert>
#include <thread>
#include "BlockQueue.hpp"

static const int MAX_ITERATION = 1000000;

void fun(TinyLib::BlockQueue<int>& queue)
{
    int value;
    for(int i=0;queue.pop(value)==0;i++)
    {
        assert(i==value);
    }
    assert(value==MAX_ITERATION-1);
}
int main()
{
    TinyLib::BlockQueue<int> queue(10); 
    auto t=std::thread(fun,std::ref(queue));
    for(int i=0;i<MAX_ITERATION;i++){
        assert(queue.push(i)==0);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    queue.exit();
    t.join();
    return 0;
}