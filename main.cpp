#include <iostream>
#include "mystring.h"

int main(int argc, char *argv[])
{
    My::String str1{"hello"};
    My::String str2{"1234567890123456789", 5};
    std::cout << "str2: "<< str2 << std::endl;
    My::String str3{"1234567890123456789"};
    std::cout << str3 << std::endl;
    My::String str4{str1};
    My::String str5{std::move(str3)};
    std::cout << " after: "<< str3 << std::endl;
    std::cout << " str5: "<< str5 << std::endl;
    return 0;
}
