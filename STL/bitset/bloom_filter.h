#pragma once 
#include"../string/string.h"
#include"./bitset.h"
using namespace Z;

struct BKDRHash
{
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		for (auto ch : s)
		{
			hash += ch;
			hash *= 31;
		}

		return hash;
	}
};

struct APHash
{
	size_t operator()(const string& s)
	{
		size_t hash = 0;
		for (unsigned long i = 0; i < s.size(); i++)
		{
			size_t ch = s[i];
			if ((i & 1) == 0)
			{
				hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
			}
			else
			{
				hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
			}
		}
		return hash;
	}
};


struct DJBHash
{
	size_t operator()(const string& s)
	{
		size_t hash = 5381;
		for (auto ch : s)
		{
			hash += (hash << 5) + ch;
		}
		return hash;
	}
};

template< size_t N, 
    class K = string,
    class Hash1 = BKDRHash,
    class Hash2 = APHash,
    class Hash3 = DJBHash>
class bloom_filter
{
public:
    void set(const K& key)
    {
        size_t len = m_X * N;
        size_t hash1 = Hash1()(key) % len;
        m_bs.set(hash1);

        size_t hash2 = Hash2()(key) % len;
		m_bs.set(hash2);

		size_t hash3 = Hash3()(key) % len;
		m_bs.set(hash3);
    }

	bool view(const K& key)const
	{
        size_t len = m_X * N;

		size_t hash1 = Hash1()(key) % len;
		if (!m_bs.view(hash1))
		{
			return false;
		}

		size_t hash2 = Hash2()(key) % len;
		if (!m_bs.view(hash2))
		{
			return false;
		}

		size_t hash3 = Hash3()(key) % len;
		if (!m_bs.view(hash3))
		{
			return false;
		}

		return true;
	}
private:
    static const size_t m_X = 6;
    bitset<m_X * N> m_bs;
};
