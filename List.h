#include "options.h"

class List
{
private:
    Options options;
    void printArguments(int argc, const char *argv[]);

public:
    void parseArguments(int argc, const char *argv[]);
    void _ls();
};