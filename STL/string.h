#include<iostream>
#include<string.h>
#include<assert.h>

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

        size_t capacity()
        {
            return m_capacity;
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
            if(n < m_size)
            {
                m_size = n;
                m_str[m_size] = '\0';
            }
            else if(n > m_size)
            {
                if(n >m_capacity)
                {
                    reserve(n);
                }

                while(m_size < n)
                {
                    m_str[m_size] = ch;
                    m_size++;
                }
                m_str[m_size] = '\0';
            }
        }
        void reserve(size_t n)
        {
            if(n > m_capacity)
            {
            char* temp = new char[n + 1];
            strcpy(temp , m_str);
            delete[] m_str;
            m_str = temp;

            m_capacity = n;
            }
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

        string& insert(size_t pos , char ch)
        {
            assert(pos <= m_size);

            if(m_size == m_capacity)
            {
                reserve(m_capacity * 2);
            }

            size_t end = m_size + 1;
            while(end > pos)
            {
                m_str[end] = m_str[end - 1];
                end--;
            }
            m_str[pos] = ch;
            m_size++;

            return *this;
        }

        string& insert(size_t pos , const char* str)
        {
            assert(pos <= m_size);
            size_t len = strlen(str);
            if(m_size + len > m_capacity)
            {
                reserve(m_size + len);
            }

            size_t end = m_size + len;
            while(end > pos + len - 1)
            {
                m_str[end] = m_str[end - len];
                end--;
            }
            strncpy(m_str + pos , str , len);
            m_size += len;

            return *this;
        }

        string& erase(size_t pos , size_t len = npos)
        {
            assert(pos <= m_size);

            if(len == npos || len >= m_size - pos)
            {
                m_str[pos] = '\0';
                m_size = pos;
            }
            else
            {
                strcpy(m_str + pos , m_str + pos + len);
                m_size -= len;
            }

            return *this;
        }

        void swap(string& s)
        {
            std::swap(m_str , s.m_str);
            std::swap(m_size , s.m_size);
            std::swap(m_capacity , s.m_capacity);
        }

        size_t find(char ch , size_t pos = 0)
        {
            assert(pos < m_size);

            for(size_t i = pos; i < m_size; i++)
            {
                if(m_str[i] == ch)
                    return i;
            }

            return npos;
        }

        size_t find(const char* str , size_t pos = 0)
        {
            assert(pos < m_size);

            char* ptr = strstr(m_str + pos , str);
            if(str == nullptr)
            {
                return npos;
            }
            return ptr - m_str;
        }

        void clear()
        {
            m_str[0] = '\0';
            m_size = 0;
        }

    private:
        char* m_str;
        size_t m_capacity;
        size_t m_size;

        static const size_t npos = -1;
    };

    std::ostream& operator<<(std::ostream& out , const string& s)
    {
        for(auto ch : s)
        {
            out << ch;
        }

        return out;
    }

    std::istream& operator>>(std::istream& in , string& s)
    {
        s.clear();

        char ch = in.get();
        char str[128];
        int i = 0;

        while(ch != ' ' && ch != '\n')
        {
            str[i++] = ch;

            if(i == 127)
            {
                str[i] = '\0';
                s += str;
                i = 0;
            }
            ch = in.get();
        }

        if(i != 0)
        {
            str[i] = '\0';
            s += str;
        }

        return in;
    }
}


