#ifndef  __WEIGOU_WORK_COLLECTOR_H__
#define  __WEIGOU_WORK_COLLECTOR_H__

#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <vector>
#include <deque>

template <typename T>
class WorkCollector {
private:
    WorkCollector(const WorkCollector& o) {};
    WorkCollector& operator=(const WorkCollector& o) {};
public:
    explicit WorkCollector();
    ~WorkCollector();
    void push(T* work);
    /**
     * @brief Block if no work complete
     *
     * @return  T* 
     * @retval   
     * @see 
     * @author yutao
     * @date 2013/07/12 15:23:44
    **/
    T* pop();
private:
    std::deque<T*> _queue;
    pthread_mutex_t _mutex_q;
    sem_t _available;
};

#include "work_collector_inl.h"

#endif  //__WORK_COLLECTOR_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
