#pragma once 
#include"../vector/vector.h"
using namespace Z;

namespace Z
{
    template<size_t N>
    class bitset
    {
    public:
        bitset()
        {
            m_bs.resize(N / 8 + 1 , 0);
        }

        void set(size_t n)
        {
            size_t i = n / 8;
            size_t j = n % 8;

            m_bs[i] |= (1 << j);
        }

        void reset(size_t n)
        {
            size_t i = n / 8;
            size_t j = n % 8;

            m_bs[i] &= ~(1 << j);
        }

        bool view(size_t n)
        {
            size_t i = n / 8;
            size_t j = n % 8;

            return m_bs[i] & (1 << j);
        }

    private:
        vector<char> m_bs;
    };
}
