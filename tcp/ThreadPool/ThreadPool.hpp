#pragma once
#include<vector>
#include<queue>
#include"Thread.hpp"
#include"LockGuard.hpp"
#include"../log.hpp"

const size_t g_thread_num = 10;

template<class T>
class ThreadPool
{
public:
    pthread_mutex_t *getMutex()
    {
        return &m_lock;
    }

    bool isEmpty()
    {
        return m_tasks.empty();
    }

    void waitCond()
    {
        pthread_cond_wait(&m_cond, &m_lock);
    }

    T getTask()
    {
        T t = m_tasks.front();
        m_tasks.pop();
        return t;
    }
private:
    ThreadPool(size_t thread_num = g_thread_num)
        :m_thread_num(thread_num)
    {
        pthread_mutex_init(&m_lock , nullptr);
        pthread_cond_init(&m_cond , nullptr);
        for(int i = 0; i < m_thread_num; i++)
        {
            m_threads.push_back(new Thread(i + 1 , routine , this));
        }
    }

    ThreadPool(const ThreadPool<T> &other) = delete;
    const ThreadPool<T> &operator=(const ThreadPool<T> &other) = delete;

public:
    static ThreadPool<T> *getThreadPool(int num = g_thread_num)
    {
        if (m_thread_ptr == nullptr) 
        {
            LockGuard lockguard(&m_mutex);
            // 但是，未来任何一个线程想获取单例，都必须调用getThreadPool接口
            // 但是，一定会存在大量的申请和释放锁的行为，这个是无用且浪费资源的
            // pthread_mutex_lock(&mutex);
            if (m_thread_ptr == nullptr)
            {
                m_thread_ptr = new ThreadPool<T>(num);
            }
            // pthread_mutex_unlock(&mutex);
        }
        return m_thread_ptr;
    }
    void run()
    {
        for(auto &iter: m_threads)
        {
            iter->start();
            logMessage(NORMAL, "%s %s", iter->name().c_str(), "启动成功");
        }
    }

    static void* routine(void* args)
    {
        ThreadData* td = (ThreadData*)args;
        ThreadPool<T>* tp = (ThreadPool<T>*)td->m_args;

        while(1)
        {
            T task;
            {
                LockGuard lg(tp->getMutex());
                while(tp->isEmpty())tp->waitCond();
                task = tp->getTask();
            }
            task(td->name);
        }
    }

    void pushTask(const T& task)
    {
        LockGuard lg(&m_lock);
        m_tasks.push(task);
        pthread_cond_signal(&m_cond);
    }

    ~ThreadPool()
    {
        for(auto &iter: m_threads)
        {
            iter->join();
            delete iter;
        }
        pthread_mutex_destroy(&m_lock);
        pthread_cond_destroy(&m_cond);
    }
private:
    size_t m_thread_num;
    std::vector<Thread*> m_threads;
    std::queue<T> m_tasks;
    pthread_mutex_t m_lock;
    pthread_cond_t m_cond;

    static ThreadPool<T>* m_thread_ptr;
    static pthread_mutex_t m_mutex;
};
template <typename T>
ThreadPool<T> *ThreadPool<T>::m_thread_ptr = nullptr;

template <typename T>
pthread_mutex_t ThreadPool<T>::m_mutex = PTHREAD_MUTEX_INITIALIZER;