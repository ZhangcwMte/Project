#pragma once 
#include<iostream>

namespace Z
{
    template<class T , class Container = vector<T> , class Compare = less<T>>
    class priority_queue
    {
    public:
        void adjust_up(size_t child)
        {
            size_t parent = (child - 1) / 2;

            while(child > 0)
            {
                if(Compare()(m_con[parent] , m_con[child]))
                {
                    swap(m_con[parent] , m_con[child]);
                    child = parent;
                    parent = (child - 1) / 2;
                }
                else 
                    break;
            }
        }

        void adjust_down(size_t parent)
        {
            size_t child = parent * 2 + 1;

            while(child < m_con.size())
            {
                if(child + 1 < m_con.size() && Compare()(m_con[child] , m_con[child + 1]))
                    child++;

                if(Compare()(m_con[parent] , m_con[child]))
                {
                    swap(m_con[parent] , m_con[child]);
                    parent = child;
                    child = parent * 2 + 1;
                }
                else 
                    break;
            }
        }

        void push(const T& val)
        {
            m_con.push_back(val);
            adjust_up(m_con.size() - 1);
        }

        void pop()
        {
            auto it = m_con.begin();
            swap(*it , *(it + m_con.size() - 1));
            m_con.pop_back();
            adjust_down(0);
        }

        const T& top()
        {
            auto it = m_con.begin();

            return *it;
        }

        size_t size()
        {
            return m_con.size();
        }

        bool empty()
        {
            return m_con.empty;
        }

    private:
        Container m_con;
    };
}
