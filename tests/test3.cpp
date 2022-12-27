#include <cassert>
#include "BlockQueue.hpp"

int main()
{
    TinyLib::BlockQueue<int> queue; 
    
    assert(queue.push(1)==0);
    queue.exit();

    assert(queue.push(2)<0);
    assert(queue.push_noneblock(2,true)<0);
    assert(queue.push_noneblock(2,false)<0);

    int value;
    assert(queue.pop(value)<0);
    assert(queue.pop_noneblock(value)<0);
    
    return 0;
}