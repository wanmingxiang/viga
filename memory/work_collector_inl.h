/***************************************************************************
 * 
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 
 
/**
 * @file work_collector_inl.h
 * @author yutao(yutao@baidu.com)
 * @date 2013/07/12 15:24:20
 * @version $Revision$ 
 * @brief 
 *  
 **/

#ifndef  __WORK_COLLECTOR_INL_H__
#define  __WORK_COLLECTOR_INL_H__

template <typename T>
WorkCollector<T>::WorkCollector()
{
    pthread_mutex_init(&_mutex_q, NULL);
    sem_init(&_available, 0, 0);
}

template <typename T>
WorkCollector<T>::~WorkCollector()
{
    sem_destroy(&_available);
    pthread_mutex_destroy(&_mutex_q);
}

template <typename T>
void WorkCollector<T>::push(T* work)
{
    pthread_mutex_lock(&_mutex_q);
    _queue.push_back(work);
    pthread_mutex_unlock(&_mutex_q);
    sem_post(&_available);
}

template <typename T>
T* WorkCollector<T>::pop()
{
    while (-1 == sem_wait(&_available));//EINTR == errno
    pthread_mutex_lock(&_mutex_q);
    T* work = _queue.front();
    _queue.pop_front();
    pthread_mutex_unlock(&_mutex_q);
    return work;
}

#endif
/* vim: set ts=4 sw=4 sts=4 tw=100 */
