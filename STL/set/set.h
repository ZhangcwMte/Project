#pragma once 
#include"../rb_tree/rb_tree.h"

namespace Z
{
    template<class K>
    class set 
    {
        struct KeyOfT
        {
            const K& operator()(const K& key)
            {
                return key;
            }
        };

        typedef typename rb_tree<K, K, KeyOfT>::const_iterator iterator;
        typedef typename rb_tree<K, K, KeyOfT>::const_iterator const_iterator;
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

        pair<iterator, bool> insert(const K& data)
        {
            return m_t.insert(data);
        }

    private:
        rb_tree<K , K , KeyOfT> m_t;
    };
}
