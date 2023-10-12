#pragma once 
#include<iostream>
#include<vector>
using namespace std;

template<class T>
struct HashNode
{
    HashNode<T>* m_next;
    T m_data;

    HashNode(const T& data)
        :m_next(nullptr)
        ,m_data(data)
    {}
};

template<class K>
struct HashFunc
{
    size_t operator()(const K& key)
    {
        return key;
    }
};

template<>
struct HashFunc<string>
{
    size_t operator()(const string& str)
    {
        size_t hash = 0;
        for(auto ch : str)
        {
            hash += ch;
            hash *= 31;
        }

        return hash;
    }
};

template<class K, class T, class KeyOfT, class Hash>
class hash_table;

template<class K, class T, class Ref, class Ptr, class KeyOfT, class Hash>
struct hash_iterator
{
    typedef HashNode<T> Node;
    typedef hash_table<K, T, KeyOfT, Hash> hash_table;
    typedef hash_iterator<K, T, Ref, Ptr, KeyOfT, Hash> Self;
    typedef hash_iterator<K, T, T&, T*, KeyOfT, Hash> iterator;

    hash_iterator(Node* node , hash_table* ht)
        :m_node(node)
        ,m_ht(ht)
    {}

    hash_iterator(const iterator& it)
        :m_node(it.m_node)
        ,m_ht(it.m_ht)
    {}

    Ref operator*()
    {
        return m_node->m_data;
    }

    Ptr operator->()
    {
        return &m_node->m_data;
    }

    Self& operator++()
    {
        if(m_node->m_next)
            m_node = m_node->m_next;
        else 
        {
            KeyOfT kot;
            Hash hash;
            size_t hash_i = hash(kot(m_node->m_data)) % m_ht->m_table.size();
            hash_i++;
            while(hash_i < m_ht->m_table.size())
            {
                if(m_ht->m_table[hash_i])
                {
                    m_node = m_ht->m_table[hash_i];
                    break;
                }
                else 
                    hash_i++;
            }

            if(hash_i == m_ht->m_table.size())
                m_node = nullptr;
        }

        return *this;
    }

    Self operator++(int)
    {
        Self ret = *this;
        if(m_node->m_next)
            m_node = m_node->m_next;
        else 
        {
            KeyOfT kot;
            Hash hash;
            size_t hash_i = hash(kot(m_node->m_data)) % m_ht->m_table.size();
            hash_i++;
            while(hash_i < m_ht->m_table.size())
            {
                if(m_ht->m_table[hash_i])
                {
                    m_node = m_ht->m_table[hash_i];
                    break;
                }
                else 
                    hash_i++;
            }

            if(hash_i == m_ht->m_table.size())
                m_node = nullptr;
        }

        return ret;
    }

    bool operator!=(const Self& it)const
    {
        return m_node != it.m_node;
    }

    bool operator==(const Self& it)const
    {
        return m_node == it.m_node;
    }

    Node* m_node;
    hash_table* m_ht;
};

template<class K, class T, class KeyOfT, class Hash>
class hash_table
{
    typedef HashNode<T> Node;
public:
    typedef hash_iterator<K, T, T&, T*, KeyOfT, Hash> iterator;
    typedef hash_iterator<K, T, const T&, const T*, KeyOfT, Hash> const_iterator;

    iterator begin()
    {
        Node* cur = nullptr;

        for(size_t i = 0; i < m_table.size(); i++)
        {
            if(m_table[i])
            {
                cur = m_table[i];
                break;
            }
        }

        return iterator(cur , this);
    }

    iterator end()
    {
        return iterator(nullptr , this);
    }

    const_iterator begin()const
    {
        Node* cur = nullptr;

        for(size_t i = 0; i < m_table.size(); i++)
        {
            if(m_table[i])
            {
                cur = m_table[i];
                break;
            }
        }

        return iterator(cur , this);
    }

    const_iterator end()const
    {
        return iterator(nullptr , this);
    }

    iterator find(const K& key)const 
    {
        if(m_table.size() == 0)
            return iterator(nullptr , this);

        KeyOfT kot;
        Hash hash;
        size_t hash_i = hash(key) % m_table.size();
        Node* cur = m_table[hash_i];

        while(cur)
        {
            if(kot(cur->m_data) == hash(key))
                return iterator(cur , this);

            cur = cur->m_next;
        }

        return iterator(cur , this);
    }

    bool erase(const K& key)
    {
        assert(m_table.size() > 0);

        KeyOfT kot;
        Hash hash;
        size_t hash_i = hash(key) % m_table.size();
        Node* cur = m_table[hash_i];
        Node* prev = nullptr;

        while(cur)
        {
            if(kot(cur->m_data) == hash(key))
            {
                if(prev)
                    prev->m_next = cur->m_next;
                else 
                    m_table[hash_i] = cur->m_next;

                delete cur;
                m_num--;

                return true;
            }
            
            prev = cur;
            cur = cur->m_next;
        }

        return false;
    }

    pair<iterator, bool> insert(const T& data)
    {
        KeyOfT kot;
        Hash hash;

        iterator it = find(data);
        if(it != end())
            return make_pair(iterator(nullptr , this), false);

        if(m_num == m_table.size())
        {
            size_t new_size = (m_table.size() == 0 ? 10 : m_table.size() * 2);
            vector<Node*> new_table(new_size , nullptr);
            
            for(auto& cur : m_table)
            {
                while(cur)
                {
                    Node* next = cur->m_next;
                    size_t hash_i = hash(kot(cur->m_data)) % m_table.size();

                    cur->next = new_table[hash_i];
                    new_table[hash_i] = cur;

                    cur = next;
                }
            }
            std::swap(m_table , new_table);
        }

        size_t hash_i = hash(kot(data)) % m_table.size();

        Node* new_node = new Node(data);
        new_node->m_next = m_table[hash_i];
        m_table[hash_i] = new_node;

        m_num++;

        return make_pair(iterator(new_node , this), true);
    }

    ~hash_table()
    {
        for(auto cur : m_table)
        {
            while(cur)
            {
                Node* next = cur->next;
                delete cur;
                cur = next;
            }
        }
    }

private:
    vector<Node*> m_table;
    size_t m_num;
};
