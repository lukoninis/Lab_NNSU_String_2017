#include <iostream>
#include "mystring.h"

int main(int argc, char *argv[])
{
    My::String str1{"hello"};
    std::cout << "str1: "<< str1 << std::endl;
    str1[2] = '!';
    std::cout << "str1: "<< str1 << std::endl;
    My::String str2{"11111111111111111111"};
    std::cout << "str2: " << str2 << std::endl;
    str2[10] = '0';
    str1 = str2;
    std::cout << "str2: " << str2 << std::endl;
    std::cout << "str1: "<< str1 << std::endl;
    return 0;
}
