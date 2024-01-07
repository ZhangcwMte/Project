#pragma once

#include<vector>
#include<queue>

#include"Log.hpp"

const size_t g_thread_num = 6;


class ThreadPool
{
    ThreadPool(int num = g_thread_num)
        :_thread_num(num), _stop(false)
    {
        pthread_mutex_init(&_lock, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }

    ThreadPool(const ThreadPool& ) = default;
public:
    static ThreadPool* getInstance()
    {
        static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        if(nullptr == single_instance)
        {
            pthread_mutex_lock(&lock);
            if(nullptr == single_instance)
            {
                single_instance = new ThreadPool();
                single_instance->initThreadPool();
            }
            pthread_mutex_unlock(&lock);
        }
        return single_instance;
    }

    bool isStop()
    {
        return _stop;
    }

    bool isQueueEmpty()
    {
        return _task_queue.empty();
    }

    void Lock()
    {
        pthread_mutex_lock(&_lock);
    }

    void unLock()
    {
        pthread_mutex_unlock(&_lock);
    }

    void threadWait()
    {
        pthread_cond_wait(&_cond, &_lock);
    }

    void threadWakeup()
    {
        pthread_cond_signal(&_cond);
    }

    static void* threadRoutine(void* args)
    {
        ThreadPool* tp = (ThreadPool*)args;

        while(1)
        {
            Task t;
            tp->Lock();
            while(tp->isQueueEmpty())
            {
                tp->threadWait();
            }
            tp->popTask(t);
            tp->unLock();
        }
    }

    bool initThreadPool()
    {
        for(int i = 0; i < _thread_num; i++)
        {
            pthread_t tid;
            if(pthread_create(&tid, nullptr, threadRoutine, this) != 0)
            {
                logMessage(FATAL, "create thread pool error!");
                return false;
            }
        }
        logMessage(NORMAL, "create thread pool success!");
        return true;
    }

    void pushTask(const Task& task)
    {
        Lock();
        _task_queue.push(task);
        unLock();
        threadWakeup();
    }

    void popTask(Task& task)
    {
        task = _task_queue.front();
        _task_queue.pop();
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_cond);
    }
private:
    int _thread_num;
    bool _stop;
    std::queue<Task> _task_queue;
    pthread_mutex_t _lock;
    pthread_cond_t _cond;

    static ThreadPool* single_instance;
};
ThreadPool* ThreadPool::single_instance = nullptr;