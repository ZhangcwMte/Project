#pragma once
#include<iostream>
#include<pthread.h>
#include<string>
#include<functional>

typedef void *(*fun_t)(void *);

class ThreadData
{
public:
    std::string name;
    void* m_args;
};

class Thread
{
public:
    Thread(int num , fun_t callback , void* args)
        :m_func(callback)
    {
        char name[64];
        snprintf(name , sizeof name , "Thread-%d" , num);
        m_name = name;

        m_data.name = m_name;
        m_data.m_args = args;
    }

    void start()
    {
        pthread_create(&m_tid , nullptr , m_func , (void*)&m_data);
    }

    void join()
    {
        pthread_join(m_tid , nullptr);
    }

    std::string name()
    {
        return m_name;
    }

    ~Thread()
    {}
private:
    pthread_t m_tid;
    std::string m_name;
    fun_t m_func;
    ThreadData m_data;
};