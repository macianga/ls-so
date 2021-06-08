#include "List.h"


//sets all except filename

int main(int argc, const char *argv[])
{
    List l;

    l.parseArguments(argc, argv);
    //const char* x = "/home/macianga/Desktop/polecenie_ls/ls.cpp";
    
    l._ls();
    return 0;
}
