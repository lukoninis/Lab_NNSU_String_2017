#include <iostream>
#include "mystring.h"

int main(int argc, char *argv[])
{
    My::String str1{"hello"};
    std::cout << "str1: "<< str1 << std::endl;
    My::String str2{"01111111111111111110"};
    std::cout << "str2: " << str2 << std::endl;

    My::String str3;
    str3 = str1 + str2;
    std::cout << "str3: " << str3 << std::endl;

    My::String str4{"hello"};
    str4 += str3;
    std::cout << "str4: " << str4 << std::endl;
    str4 += "asdaf";
    std::cout << "str4: " << str4 << std::endl;
    return 0;
}
