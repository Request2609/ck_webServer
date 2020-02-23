#include <iostream>
#include <unistd.h>
#include<fcntl.h>

int main()
{
    std::cout << access("a.CGI", F_OK)<<std::endl ;
    return 0;
}

