#pragma once 
#include<mutex>

namespace Z
{
    template<class T>
    class shared_ptr
    {
    public:
        shared_ptr(T* ptr)
            :m_ptr(ptr)
            ,m_pcount(new size_t(1))
            ,m_pmtx(new std::mutex)
        {}

        shared_ptr(const shared_ptr<T>& sp)
            :m_ptr(sp.m_ptr)
            ,m_pcount(sp.m_pcount)
            ,m_pmtx(sp.m_pmtx)
        {
            addCount();
        }
        
        void release()
        {
            m_pmtx->lock();

            if(m_pcount == 0)
            {
                delete m_ptr;
                delete m_pcount;
            }

            m_pmtx->unlock();
        }

        void addCount()
        {
            m_pmtx->lock();

            *(m_pcount)++;

            m_pmtx->unlock();
        }

        T& operator*()
		{
			return *m_ptr;
		}

		T* operator->()
		{
			return m_ptr;
		}

		T* get()
		{
			return m_ptr;
		}

		int use_count()
		{
			return *m_pcount;
		}

        ~shared_ptr()
        {
            release();
        }

    private:
        std::mutex* m_pmtx;
        T* m_ptr;
        size_t* m_pcount;
    };
}
