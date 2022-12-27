#include <cassert>
#include "BlockQueue.hpp"


int main()
{
    TinyLib::BlockQueue<int> queue(1);
    
    assert(queue.push(1)==0);
    assert(queue.full());
    assert(queue.push_noneblock(2,false)<0);
    assert(queue.push_noneblock(2,true)==0);

    int value;
    assert(queue.pop(value)==0 && value==2);
    
    return 0;
}