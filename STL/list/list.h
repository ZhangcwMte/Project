#pragma once 
#include<iostream>
#include<assert.h>

namespace Z
{
    template<class T>
    struct list_node
    {
        list_node<T>* m_prev;
        list_node<T>* m_next;
        T m_data;

        list_node(const T& val  = T())
            :m_prev(nullptr),
             m_next(nullptr),
             m_data(val)
        {}
    };

    template<class T , class Ref , class Ptr>
    struct __list_iterator
    {
        typedef __list_iterator<T , Ref , Ptr> self;
        typedef list_node<T> node;

        __list_iterator(node* n)
            :m_node(n)
        {}

        Ref operator*()
        {
            return m_node->m_data;
        }

        Ptr operator->()
        {
            return &m_node->m_data;
        }

        self& operator++()
        {
            m_node = m_node->m_next;

            return *this;
        }

        self operator++(int)
        {
            node* temp = m_node;
            m_node = m_node->m_next;

            return temp;
        }

        self& operator--()
        {
            m_node = m_node->m_prev;

            return *this;
        }

        self operator--(int)
        {
            node* temp = *this;
            m_node = m_node->m_prev;

            return temp;
        }

        bool operator!=(const self& it)
        {
            return m_node != it.m_node;
        }

        bool operator==(const self& it)
        {
            return m_node == it.m_node;
        }

        node* m_node;
    };

    template<class T>
    class list 
    {
        typedef list_node<T> node;
    public:
        typedef __list_iterator<T , T& , T*> iterator;
        typedef __list_iterator<T , const T& , const T*> const_iterator;

        void empty_init()
        {
            m_head = new node;
            m_head->m_prev = m_head;
            m_head->m_next = m_head;
        }

        list()
        {
            empty_init();
        }

        list(const list<T>& lt)
        {
            empty_init();

            for(auto val : lt)
            {
                push_back(val);
            }
        }

        template<class InputIterator>
        list(InputIterator begin , InputIterator end)
        {
            empty_init();

            while(begin != end)
            {
                push_back(*(begin++));
            }
        }

        iterator begin()
        {
            return m_head->m_next;
        }

        iterator end()
        {
            return m_head;
        }

        const_iterator begin()const 
        {
            return m_head->m_next;
        }

        const_iterator end()const 
        {
            return m_head;
        }

        void swap(list<T>& lt)
        {
            std::swap(m_head , lt.m_head);
        }

        list<T>& operator=(list<T> lt)
        {
            swap(lt);

            return *this;
        }

        void push_back(const T& val)
        {
            insert(end() , val);
        }

        void push_front(const T& val)
        {
            insert(begin() , val);
        }

        void pop_back()
        {
            erase(--end());
        }

        void pop_front()
        {
            erase(begin());
        }

        void insert(const iterator& pos , const T& val)
        {
            node* prev = pos.m_node->m_prev;
            node* next = pos.m_node;

            node* new_node = new node(val);

            new_node->m_prev = prev;
            new_node->m_next = next;
            prev->m_next = new_node;
            next->m_prev = new_node;
        }

        iterator erase( iterator pos)
        {
            assert(pos != end());

            node* prev = pos.m_node->m_prev;
            node* next = pos.m_node->m_next;

            prev->m_next = next;
            next->m_prev = prev;

            delete pos.m_node;

            return iterator(next);
        }

        void clear()
        {
            iterator it = begin();
            while(it != end())
            {
                erase(it++);
            }
        }

        ~list()
        {
            clear();
            delete m_head;
            m_head = nullptr;
        }

    private:
        node* m_head;
    };
}
