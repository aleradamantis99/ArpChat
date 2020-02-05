#include <cstring>
#include <iostream>
#include <variant>

struct A
{
    char str[3];
};

struct B
{
    unsigned short num;
    char a;
};

using data = std::variant<A, B>;

struct S
{
    char a, b;
    data d;
};

int main()
{
    S var1 {'A', 'B'};
    
    strncpy (std::get<A>(var1.d).str, "CD", 3);
    
    S* var = &var1;
    //std::cout << var1.a << var1.b << std::endl;
    for (int i=0; i<10; ++i)
        std::cout << *((char*)var+i) << std::endl;
    //std::cout << *((char*)var) << std::endl;
    return 0;
}
