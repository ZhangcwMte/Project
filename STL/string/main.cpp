#include"string.h"
using namespace std;

int main()
{
    Z::string s = "Hello World!";
    s.insert(0 , "No");
    s.erase(0 , 2);
    int n = s.find ('H');
    
    cout << s << endl;
    cout << s[6] << endl;
    cout << n << endl;

    return 0;
}
