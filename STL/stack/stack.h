#pragma once

namespace Z
{
    template<class T , class Container = vector<T>>
    class stack 
    {
    public:
        void push(const T& val)
        {
            m_con.push_back(val);
        }

        void pop()
        {
            m_con.pop_back();
        }

        const T& top()
        {
            return m_con.back();
        }

        size_t size()
        {
            return m_con.size();
        }

        bool empty()
        {
            return m_con.empty();
        }

    private:
        Container m_con;
    };
}
