#pragma once
#include<iostream>
#include<assert.h>

namespace Z
{
    template<class T>
    class vector 
    {
    public:
        typedef T* iterator;
        typedef const T* const_iterator;

        vector()
        {
            ;
        }

        vector(size_t n , const T& val = T())
        {
            reserve(n);

            for(size_t i = 0; i < n; i++)
            {
                push_back(val);
            }
        }
        
        vector(int n , const T& val = T())
        {
            reserve(n);

            for(int i = 0; i < n; i++)
            {
                push_back(val);
            }
        }

        template<class InputIterator>
        vector(InputIterator begin, InputIterator end)
        {
            while(begin != end)
                push_back(*(begin++));
        }

        vector(const vector<T>& v)
        {
            reserve(v.capacity());
            for(int i = 0; i < v.size(); i++)
            {
                *(m_start + i) = *(v.m_start + i);
            }
            m_finish = m_start + v.size();
            m_end_of_storage = m_start + v.capacity();
        }

        iterator begin()
        {

            return m_start;
        }

        iterator end()
        {
            return m_finish;
        }

        const_iterator begin()const
        {
            return m_start;
        }

        const_iterator end()const 
        {
            return m_finish;
        }

        size_t size()const
        {
            return m_finish - m_start;
        }

        size_t capacity()const
        {
            return m_end_of_storage - m_start;
        }

        void resize(size_t n , const T& val = T())
        {
            if (n < size())
                m_finish = m_start + n;
            else if(n > capacity())
                reserve(n);

            while(m_finish != m_start + n)
                *(m_finish++) = val;
        }

        bool empty()
        {
            return m_finish == m_finish;
        }

        void reserve(size_t n)
        {
            T* temp = new T[n];
            size_t sz = size();
            if(m_start)
            {
                for(auto val : m_start)
                {
                    *(temp++) = val;
                }
                delete[] m_start;
            }
            m_start = temp;
            m_finish = m_start + sz;
            m_end_of_storage = m_start + n;
        }

        vector<T>& operator=(const vector<T>& v)
        {
            m_start = m_finish = new T[v.capacity()];
            for(int i = 0; i < v.size(); i++)
            {
                *(m_finish++) = *(v.m_start + i);
            }
            m_end_of_storage = m_start + v.capacity();

            return *this;
        }

        vector<T>& operator[](size_t n)
        {
            assert(n < size());

            return *(m_start + n);
        }

        const vector<T>& operator[](size_t n)const 
        {
            assert(n < size());

            return *(m_start + n);
        }

        void push_back(const T& val)
        {
            if(size() == capacity())
                reserve(capacity() == 0 ? 4 : 2 * capacity());
            *(m_finish++) = val;
        }

        void pop_back()
        {
            assert(!empty());

            m_finish--;
        }

        iterator insert(iterator pos , const T& val)
        {
            assert(pos >= m_start);
            assert(pos <= m_finish);

            if(size() == capacity())
            {
                size_t len = pos - m_start;
                reserve(capacity() == 0 ? 4 : 2 * capacity());
                pos = m_start + len;
            }

            T* end = m_finish;
            while(end != pos)
            {
                *end = *(end - 1);
                end--;
            }
            *pos = val;
            m_finish++;

            return pos;
        }

        iterator erase(iterator pos)
        {
            assert(!empty());
            assert(pos >= m_start);
            assert(pos < m_finish);

            T* end = pos + 1;
            while(end != m_finish)
            {
                *(end - 1) = *end;
                end++;
            }
            m_finish--;

            return pos;
        }

        ~vector()
        {
            delete[] m_start;
            m_start = m_finish = m_end_of_storage = nullptr;
        }

    private:
        iterator m_start = nullptr;
        iterator m_finish = nullptr;
        iterator m_end_of_storage = nullptr;
    };
}
