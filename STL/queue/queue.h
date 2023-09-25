#pragma once

namespace Z
{
    template<class T , class Container = list<T>>
    class queue 
    {
    public:
        void push(const T& val)
        {
            m_con.push_back(val);
        }

        void pop()
        {
            m_con.erase(m_con.begin());
        }

        const T& front()
        {
            return m_con.front();
        }

        const T& back()
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
