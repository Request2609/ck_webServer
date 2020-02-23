#include <iostream>
#include <string.h>
#include <stdio.h>

void getArg(int& a, int& b, const std::string arg) {
    sscanf(arg.data(), "a=%d&b=%d", &a, &b) ;
}


int main(int argc, char** argv) { 
    int a, b ;
    std::string arg = argv[0] ;
    getArg(a, b, arg) ;
    int t = a+b;
    std::string body = "<html><meta charset=\"UTF-8\"><head><title>CK_webServerCGI</title></head>";
    body += "<body><p>The result is " + std::to_string(a) + "+" + std::to_string(b) + " = " + std::to_string(t);
    body += "</p></body></html>";
    int size = body.size() ;
    body = std::to_string(size)+"\r\n"+body ;
    printf("%s", body.c_str()) ;
    return 0;
}

