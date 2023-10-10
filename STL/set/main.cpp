#include<iostream>

#include"set.h"
using namespace Z;

int main()
{
    set<int> st;

    st.insert(4);
    st.insert(9);
    st.insert(5);
    st.insert(1);

    for(auto n : st)
    {
        std::cout << n;
    }
    std::cout << std::endl;

    return 0;
}
