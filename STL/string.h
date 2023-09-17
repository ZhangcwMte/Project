#include<iostream>
#include<string.h>
#include<assert.h>
using std::cout;
using std::endl;

namespace Z
{
    class string
    {
    public:
        typedef char* iterator;
        typedef const char* const_iterator;

        iterator begin()
        {
            return m_str;
        }

        iterator end()
        {
            return m_str + m_size;
        }

        const_iterator begin()const
        {
            return m_str;
        }

        const_iterator end()const
        {
            return m_str + m_size;
        }
        string(const char* str = "")
            :m_size(strlen(str))
        {
            m_capacity = m_size == 0 ? 3 : m_size;
            m_str = new char[m_capacity + 1];
            strcpy(m_str , str);
        }

        string(const string& s)
            :m_size(s.m_size),
             m_capacity(s.m_capacity)
        {
            m_str = new char[m_capacity + 1];
            strcpy(m_str , s.m_str);
        }

        string& operator=(const string& s)
        {
            if(this != &s)
            {
                m_size = s.m_size;
                m_capacity = s.m_capacity;
                m_str = new char[m_capacity + 1];
                strcpy(m_str , s.m_str);
            }
            return *this;
        }

        ~string()
        {
            delete[] m_str;
            m_str = nullptr;
            m_size = m_capacity = 0;
        }

        const char* c_str()
        {
            return m_str;
        }

        char& operator[](const int pos)
        {
            assert(pos < m_size);
            return m_str[pos];
        }

        char& operator[](const int pos)const
        {
            assert(pos < m_size);
            return m_str[pos];
        }

        size_t size()
        {
            return m_size;
        }

        bool operator>(const string& s)const
        {
            return strcmp(m_str , s.m_str) > 0;
        }

        bool operator==(const string& s)const
        {
            return strcmp(m_str , s.m_str) == 0;
        }

        bool operator>=(const string& s)const
        {
            return *this > s || *this == s;
        }

        bool operator<(const string& s)const
        {
            return !(*this >= s);
        }

        bool operator<=(const string& s)const
        {
            return !(*this > s);
        }

        void resize(size_t n , char ch = '\0')
        {

        }

        void reserve(size_t n)
        {
            char* temp = new char[n + 1];
            strcpy(temp , m_str);
            delete[] m_str;
            m_str = temp;

            m_capacity = n;
        }
        
        void push_back(char ch)
        {
            if(m_size == m_capacity)
            {
                reserve(m_capacity * 2);
            }
            m_str[m_size] = ch;
            m_size++;
            m_str[m_size] = '\0';
        }

        void append(const string& s)
        {
            int len = s.m_size;
            if(m_size + len > m_capacity)
            {
                reserve(m_size + len + 1);
            }
            strcpy(m_str + m_size , s.m_str);
            m_size += len;
        }

        string& operator+=(const string& s)
        {
            append(s);
            return *this;
        }

        string& operator+=(char ch)
        {
            push_back(ch);
            return *this;
        }
    private:
        char* m_str;
        size_t m_capacity;
        size_t m_size;

        static const size_t npos = -1;
    };
}
