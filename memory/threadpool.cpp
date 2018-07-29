#include <stdlib.h>
#include "threadpool.h"

using namespace std;

ThreadPool::ThreadPool(): _max_threads(0), _pool_threads(NULL)
{}

ThreadPool::ThreadPool(int max_threads): _max_threads(0), _pool_threads(NULL)
{
    if (init(max_threads) < 0) {
        throw "ThreadPool Initialization Exception";
    }
}

int ThreadPool::init(int max_threads)
{
    if (_max_threads) {
        return -1;
    }
    if (max_threads < 1) {
        max_threads = 1;
    }
    _max_threads = max_threads;
    //mutexSync = PTHREAD_MUTEX_INITIALIZER;
    //_mutex_q = PTHREAD_MUTEX_INITIALIZER; 
    pthread_mutex_init(&_mutex_q, NULL);
    pthread_rwlock_init(&_rwlock, NULL);
    sem_init(&_available_work, 0, 0);

    _pool_threads = new pthread_t[_max_threads];
    for (int i = 0; i < _max_threads; ++i)
    {
        pthread_create(_pool_threads + i, NULL, &ThreadPool::thread_run, (void *)this); 
        //threadIdVec[i] = tempThread;
    }
    return 0;
}

ThreadPool::~ThreadPool()
{
    sem_destroy(&_available_work);
    pthread_mutex_destroy(&_mutex_q);
    pthread_rwlock_destroy(&_rwlock);
    delete[] _pool_threads;
}

int ThreadPool::assign_work(WorkerThread *workerThread)
{
    if (!_max_threads || !workerThread) {
        return -1;
    }
    pthread_rwlock_rdlock(&_rwlock);
    pthread_mutex_lock(&_mutex_q);
    _input_queue.push_back(workerThread);
    //cout << "assign_work...incomapleteWork=" << incompleteWork << endl;
    sem_post(&_available_work);
    pthread_mutex_unlock(&_mutex_q);
    pthread_rwlock_unlock(&_rwlock);
    return 0;
}

int ThreadPool::assign_work_batch(WorkerThread* workerThread[], int num)
{
    if (!_max_threads || !workerThread) {
        return -1;
    }
    pthread_rwlock_rdlock(&_rwlock);
    int ret = 0;
    pthread_mutex_lock(&_mutex_q);
    for (int i = 0; i < num; ++i) {
        if (!workerThread[i]) {
            continue;
        }
        _input_queue.push_back(workerThread[i]);
        sem_post(&_available_work);
        ++ret;
        //cout << "assign_work...incomapleteWork=" << incompleteWork << endl;
    }
    pthread_mutex_unlock(&_mutex_q);
    pthread_rwlock_unlock(&_rwlock);
    return ret;
}

int ThreadPool::run()
{
    while (1) {
        while (-1 == sem_wait(&_available_work)); //EINTR == errno
        pthread_rwlock_wrlock(&_rwlock);
        //pthread_mutex_lock(&_mutex_q);
        WorkerThread *worker = _input_queue.front();
        _input_queue.pop_front();
        //pthread_mutex_unlock(&_mutex_q);
        pthread_rwlock_unlock(&_rwlock);
        worker->run();
        //cout << "worker[" << worker->id << "]\tdelete address: [" << worker << "]" << endl;
    }
    return 0;
}

void* ThreadPool::thread_run(void* thd_data)
{
    ThreadPool* thd_pool = reinterpret_cast<ThreadPool*>(thd_data);
    thd_pool->run();
    pthread_exit((void*) 0);
}
