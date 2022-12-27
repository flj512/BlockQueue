# BlockQueue
A block queue implementation for c++ with only a single header file.  
provide block&none-block interfaces.

# Example
```
#include <thread>
#include "BlockQueue.hpp"

static const int MAX_ITERATION = 1000;

void fun(TinyLib::BlockQueue<int>& queue)
{
    int value;
    while(queue.pop(value)==0);
}
int main()
{
    TinyLib::BlockQueue<int> queue(10); 
    auto t=std::thread(fun,std::ref(queue));
    for(int i=0;i<MAX_ITERATION;i++){
        queue.push(i);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    queue.exit();
    t.join();
    return 0;
}
```
