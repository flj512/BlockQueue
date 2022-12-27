/**
 * @file BlockQueue.hpp
 * @author flj
 * @brief A block queue implementation for c++ with only a single header file,support block or none-block mode.
 * @version 0.1
 * @date 2022-12-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

#include<mutex>
#include<queue>
#include<chrono>
#include<condition_variable>

namespace TinyLib{
template<typename T>
class BlockQueue{
public:
/**
 * @brief Construct a new Block Queue object
 * 
 * @param max_size if max_size < 0 then the max size of the queue is infinity
 */
    BlockQueue(size_t max_size=-1):_exited(false),_max_size(max_size)
    {
    }
    ~BlockQueue()
    {
        exit();
    }
/**
 * @brief push an item to the queue, if the queue is full this function will blocked.
 *
 * 
 * @param t the input data.
 * @param timeout the max wait time, the unit is second.
 * @return int return -1 if the queue is exited otherwise return 0
 */
    int push(const T& t,float timeout = 0.0)
    {
        return _push_impl(t,true,false,timeout);
    }
/**
 * @brief push an item to the queue, if the queue is full return fail or remove the oldest data and put the new one.
 * 
 * @param t the input data.
 * @param drop_oldest indicate return fail or remove the oldest data when the queue is full.
 * @return int return 0 if success otherwise return -1
 */
    int push_noneblock(const T& t,bool drop_oldest=true)
    {
        return _push_impl(t,false,drop_oldest);
    }
/**
 * @brief get an item from the queue, if the queue is empty this function will blocked.
 * 
 * @param t the output data.
 * @param timeout the max wait time, the unit is second.
 * @return int return 0 if success otherwise return -1
 */
    int pop(T& t,float timeout = 0.0)
    {
        return _pop_impl(t,true,timeout);
    }
/**
 * @brief get an item from the queue, if the queue is empty this function will return immediately.
 * 
 * @param t the output data.
 * @return int return 0 if success otherwise return -1
 */
    int pop_noneblock(T& t)
    {
        return _pop_impl(t,false);
    }
/**
 * @brief exit the queue, all the push/pop function will fail after exit and all the
 * threads that wait on this queue will wakeup.
 * 
 */
    void exit();
/**
 * @brief clear all data in the queue.
 * 
 */
    void clear();
/**
 * @brief get the size of the queue.
 * 
 * @return * size_t 
 */
    size_t size();
/**
 * @brief return whether the queue is full 
 * 
 * @return true 
 * @return false 
 */
    bool full();
private:
    int _push_impl(const T& t,bool is_block,bool drop_oldest,float timeout=0.0);
    int _pop_impl(T& t,bool is_block,float timeout=0.0);
    bool _idle()
    {
        return (_buffer.size() < _max_size) || _exited;
    }
    bool _has_item()
    {
        return _buffer.size()>0 || _exited;        
    }

    bool _exited;
    size_t _max_size;
    std::queue<T> _buffer;
    std::mutex _buffer_mtx;
    std::condition_variable _read_condition;
    std::condition_variable _write_condition;
};

template<typename T>
void BlockQueue<T>::exit()
{
    {
        std::unique_lock<std::mutex> lock(_buffer_mtx);
        _exited = true;
    }
    _read_condition.notify_all();
    _write_condition.notify_all();
}
template<typename T>
void BlockQueue<T>::clear()
{
    {
        std::unique_lock<std::mutex> lock(_buffer_mtx);
        _buffer.clear();
    }
    _write_condition.notify_all();
}
template<typename T>
size_t BlockQueue<T>::size()
{
    std::unique_lock<std::mutex> lock(_buffer_mtx);
    return _buffer.size();
}

template<typename T>
bool BlockQueue<T>::full()
{
    std::unique_lock<std::mutex> lock(_buffer_mtx);
    return (_max_size>0 && _buffer.size()>=_max_size);
}

template<typename T>
int BlockQueue<T>::_push_impl(const T& t,bool is_block,bool drop_oldest,float timeout)
{
    bool notify = false,dropped = false;
    {
        std::unique_lock<std::mutex> lock(_buffer_mtx);
        if(_exited){
            return -1;
        }
        if(_max_size>0 && _buffer.size() >= _max_size){
            if(!is_block && !drop_oldest){
                return -1;
            }
            if(drop_oldest){//is_block = True, drop_oldest always = false
                _buffer.pop();
                dropped = true;
            }else{
                if (timeout <= 0){
                    _write_condition.wait(lock,[this]{
                        return _idle();
                    });
                }else{
                    if(!_write_condition.wait_for(lock,std::chrono::duration<float>(timeout),[this]{
                        return _idle();
                    })){
                        return -1;
                    }
                }
                if(_exited){
                    return -1;
                }
            }
        }
        _buffer.push(t);
        notify = (_buffer.size()==1);
    }

    if(notify && !dropped){
        _read_condition.notify_all();
    }
    return 0;
}
template<typename T>
int BlockQueue<T>::_pop_impl(T& t,bool is_block,float timeout)
{
    bool notify = false;
    {
        std::unique_lock<std::mutex> lock(_buffer_mtx);
        if(_exited){
            return -1;
        }

        if(_buffer.size()==0){
            if(!is_block){
                return -1;
            }
            if (timeout <= 0){
                _read_condition.wait(lock,[this]{
                    return _has_item();
                });
            }else{
                if(!_read_condition.wait_for(lock,std::chrono::duration<float>(timeout),[this]{
                    return _has_item();
                })){
                    return -1;
                }
            }
            if(_exited){
                return -1;
            }
        }

        t = std::move(_buffer.front());
        _buffer.pop();
        notify = (_max_size>0 && (_buffer.size()==_max_size-1));      
    }
    if(notify){
        _write_condition.notify_all();
    }
    return 0;
}
}