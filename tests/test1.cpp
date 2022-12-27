#include <cassert>
#include "BlockQueue.hpp"

int main()
{
    TinyLib::BlockQueue<int> queue;
    
    assert(queue.push(1)==0);
    assert(queue.push(2)==0);
    assert(queue.push(3)==0);

    int value;
    assert(queue.pop(value)==0 && value==1);
    assert(queue.pop(value)==0 && value==2);
    assert(queue.pop(value)==0 && value==3);

    assert(queue.pop_noneblock(value)<0);
    return 0;
}