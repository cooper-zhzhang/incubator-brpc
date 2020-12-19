#include <iostream>

#include <bvar/bvar.h>

int main(int argc, char* argv[])
{

    bvar::Adder<int> g_read_error("", "name") ;

    std::cout << g_read_error.name()  << std::endl;

    g_read_error << 1 << 2;

    return 0;
}
