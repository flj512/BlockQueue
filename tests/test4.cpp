#include <cassert>
#include "BlockQueue.hpp"

int main()
{
    TinyLib::BlockQueue<int> queue(1); 
    
    assert(queue.push(1)==0);

    assert(queue.push(2,0.52)<0);


    int value;
    assert(queue.pop(value)==0&&value==1);
    assert(queue.pop(value,0.43)<0);
    
    return 0;
}