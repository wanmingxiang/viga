/***********************************************
*      filename: delaypool.cpp
*      version: ---
*      
*      @author: wanmx@foxmail.com
*      @description: ---
*      @create: 2017-05-10 02:25:32
*      @last Modified: 2017-05-10 02:26:01
************************************************/

#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "delaypool.h"

volatile uint32_t g_now_time;

static void *pool_timer_thread(void * /*ptr*/)
{
    pthread_detach(pthread_self());

    while (1)
    {
        g_now_time = ::time(NULL);
        ::usleep(500*1000);
    }
    return NULL;
}

void init_time_updater()
{
    g_now_time = ::time(NULL);

    pthread_t pid;
    int ret = ::pthread_create(&pid, NULL, pool_timer_thread, NULL);
    if (ret != 0)
    {
        ::exit(-1);
    }
}
