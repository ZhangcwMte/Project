#pragma once 
#include"../hash/hash_bucket.h"

namespace Z
{
    template<class K, class V, class Hash = HashFunc<K>>
    class unordered_map
    {
        struct KeyOfT 
        {
            const K& operator()(const pair<K, V>& data)
            {
                return data.first;
            }
        };

        typedef typename hash_table<K, pair<const K, V>, KeyOfT, Hash>::iterator iterator;
        typedef typename hash_table<K, pair<const K, V>, KeyOfT, Hash>::const_iterator const_iterator;

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

        pair<iterator, bool> insert(const pair<K, V>& data)
        {
            return m_ht.insert(data);
        }

        V& operator[](const K& key)
        {
            return insert(make_pair(key , V())).first->second;
        }

    private:
        hash_table<K, pair<const K, V>, KeyOfT, Hash> m_ht;
    };
}

