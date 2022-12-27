#include <cassert>
#include "BlockQueue.hpp"

int main()
{
    TinyLib::BlockQueue<int> queue;
    
    assert(queue.push(1)==0);
    assert(queue.push(2)==0);
    assert(queue.push(3)==0);
    queue.clear();
    assert(queue.size()==0);

    return 0;
}