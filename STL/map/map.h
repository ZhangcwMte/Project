#pragma once 
#include"../rb_tree/rb_tree.h"

namespace Z
{
    template<class K , class V>
    class map
    {
        struct KeyOfT
        {
            const K& operator()(const pair<K, V>& data)
            {
                return data.first;
            }
        };

        typedef typename rb_tree<K, pair<K, V>, KeyOfT>::iterator iterator;
        typedef typename rb_tree<K, pair<K, V>, KeyOfT>::const_iterator const_iterator;

    public:
        iterator begin()
        {
            return m_t.begin();
        }

        iterator end()
        {
            return m_t.end();
        }

        const_iterator begin()const
        {
            return m_t.begin();
        }

        const_iterator end()const
        {
            return m_t.end();
        }

        V& operator[](const K& key)
        {
            return m_t.insert(make_pair(key , V())).first->second;
        }

        pair<iterator, bool> insert(const pair<K, V>& data)
        {
            return m_t.insert(data);
        }

    private:
        rb_tree<K, pair<const K, V>, KeyOfT> m_t;
    };
}
