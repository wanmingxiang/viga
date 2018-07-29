#ifndef __WEIGOU_THREAD_POOL_H__
#define __WEIGOU_THREAD_POOL_H__

#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <vector>
#include <deque>

using namespace std;
/*
WorkerThread class
This class needs to be subclassed by the user.
*/
class WorkerThread{
public:
    int virtual run() = 0;
    virtual ~WorkerThread() {}
};

/*
ThreadPool class manages all the ThreadPool related activities. This includes keeping track of idle threads and ynchronizations between all threads.
*/
class ThreadPool{
private:
    ThreadPool(const ThreadPool& o);
    ThreadPool& operator=(const ThreadPool& o);
public:
    ThreadPool();
    explicit ThreadPool(int max_threads);
    virtual ~ThreadPool();
    int init(int max_threads);
	
    int assign_work(WorkerThread *worker);
    int assign_work_batch(WorkerThread* workers[], int num);
    
    int run();

    static void* thread_run(void* thd_data);
    
private:
    int _max_threads;
    pthread_t *_pool_threads;

    pthread_mutex_t _mutex_q;
    sem_t _available_work;
    pthread_rwlock_t _rwlock; //while working thread aquires write lock

    std::deque<WorkerThread *> _input_queue;
};

#endif


