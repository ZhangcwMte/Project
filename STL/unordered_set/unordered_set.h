#pragma once 
#include"../hash/hash_bucket.h"

namespace Z
{
    template<class K, class Hash = HashFunc<K>>
    class unordered_set
    {
        struct KeyOfT 
        {
            const K& operator()(const K& key)
            {
                return key;
            }
        };

        typedef typename hash_table<K, K, KeyOfT, Hash>::const_iterator iterator;
        typedef typename hash_table<K, K, KeyOfT, Hash>::const_iterator const_iterator;

    public:
        iterator begin()
        {
            return m_ht.begin();
        }

        iterator end()
        {
            return m_ht.end();
        }

        const_iterator begin()const
        {
            return m_ht.begin();
        }

        const_iterator end()const
        {
            return m_ht.end();
        }

        iterator find(const K& key)const 
        {
            return m_ht.find(key);
        }

        bool erase(const K& key)
        {
            return m_ht.erase(key);
        }

        pair<iterator, bool> insert(const K& key)
        {
            return m_ht.insert(key);
        }

    private:
        hash_table<K, K, KeyOfT, Hash> m_ht;
    };
}

