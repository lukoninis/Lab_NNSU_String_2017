#include "string.h"

int main(int argc, char *argv[])
{
    My::String str1{"hello"};
    My::String str2;
    My::String str3{"1234567890123456789"};
    My::String str4{str1};
    My::String str5{std::move(str3)};
    return 0;
}
