#pragma once 

namespace Z
{
    template<class iterator , class Ref , class Ptr>
    struct reverse_iterator
    {
        typedef reverse_iterator<iterator , Ref , Ptr> Self;

        reverse_iterator(iterator it)
            :m_cur(it)
        {
            ;
        }

        Ref operator*()
        {
            return *(m_cur - 1);
        }

        Self& operator++()
        {
            m_cur--;

            return *this;
        }

        Self operator++(int)
        {
            Self temp = *this;
            m_cur--;

            return temp;
        }

        Self& operator--()
        {
            m_cur++;

            return *this;
        }

        Self operator--(int)
        {
            Self temp = *this;
            m_cur++;

            return temp;
        }

        bool operator!=(const Self& it)
        {
            return m_cur != it.m_cur;
        }

        bool operator==(const Self& it)
        {
            return m_cur == it.m_cur;
        }

        iterator m_cur;
    };
}
