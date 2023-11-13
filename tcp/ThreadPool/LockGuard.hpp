#pragma once
#include<iostream>
#include<pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t* mtx)
        :m_pmtx(mtx)
    {}

    void lock()
    {
        pthread_mutex_lock(m_pmtx);
    }

    void unlock()
    {
        pthread_mutex_unlock(m_pmtx);
    }

    ~Mutex()
    {}
private:
    pthread_mutex_t* m_pmtx;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t* mtx)
        :m_mtx(mtx)
    {
        m_mtx.lock();
    }

    ~LockGuard()
    {
        m_mtx.unlock();
    }
private:
    Mutex m_mtx;
};