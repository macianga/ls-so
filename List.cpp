#include "List.h"
//Used for basic input/output stream
#include <stdio.h>
//Used for handling directory files
#include <dirent.h>
//For EXIT codes and error handling
#include <errno.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include "fileInfo.h"

#include <iostream>

void List::parseArguments(int argc, const char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        //check if argument is an option
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'l')
            {
                options.l = true;
            }
            if (argv[i][1] == 'r')
            {
                options.r = true;
            }
            if (argv[i][1] == 'a')
            {
                options.a = true;
            }
            if (argv[i][1] == 'f')
            {
                options.f = true;
            }
            if (argv[i][1] == 'g')
            {
                options.g = true;
            }
            if (argv[i][1] == 'm')
            {
                options.m = true;
            }
        }
        else if (argv[i][0] == '/' || argv[i][0] == '.')
        {
            options.dir = argv[i];
        }
    }
}

void List::printArguments(int argc, const char *argv[])
{
    printf("Number of arguments: %i\n", argc);
    for (int i = 0; i < argc; i++)
    {
        //std::cout << "arg nr " << argc << ": " << argv[i] << std::endl;
        printf("arg %i: ", i);
        printf("%s", argv[i]);
        printf("\n");
    }
}

bool compareFunctionNormal(fileInfo file1, fileInfo file2)
{
    std::string a = file1.fileName;
    std::string b = file2.fileName;
    std::for_each(a.begin(), a.end(), [](char &c)
                  { c = ::toupper(c); });
    std::for_each(b.begin(), b.end(), [](char &c)
                  { c = ::toupper(c); });
    return a < b;
}

bool compareFunctionReverse(fileInfo file1, fileInfo file2)
{
    std::string a = file1.fileName;
    std::string b = file2.fileName;
    std::for_each(a.begin(), a.end(), [](char &c)
                  { c = ::toupper(c); });
    std::for_each(b.begin(), b.end(), [](char &c)
                  { c = ::toupper(c); });
    return a > b;
}

void List::_ls()
{
    //Here we will list the directory
    struct dirent *d;
    DIR *dh = opendir(this->options.dir);
    if (!dh)
    {
        if (errno = ENOENT)
        {
            //If the directory is not found
            perror("Directory doesn't exist");
        }
        else
        {
            //If the directory is not readable then throw error and exit
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }
    //While the next entry is not readable we will print directory files

    std::vector<fileInfo> entries;

    while ((d = readdir(dh)) != NULL)
    {
        //If hidden files are found we continue
        if (!options.a && d->d_name[0] == '.')
            continue;

        //printf("%s  ", d->d_name);
        //entries.push_back(d->d_name);
        fileInfo fi = fileInfoFunctions::getFileInfo(this->options.dir, d->d_name);
        entries.push_back(fi);
        //entries.push_back(d->d_name);
        //if(this->options.l) printf("\n");
    }

    if (!this->options.f)
    {
        if (this->options.r)
        {
            std::sort(entries.begin(), entries.end(), compareFunctionReverse); //sort the vector
        }
        else
        {
            std::sort(entries.begin(), entries.end(), compareFunctionNormal); //sort the vector
        }
    }

    if (this->options.m)
    {
        for (auto en : entries)
        {
            std::cout << en.fileName << ", ";
        }
        std::cout << std::endl;
    }
    else if (this->options.l)
    {
        for (auto en : entries)
        {
            std::cout << fileInfoFunctions::fileInfo_toString(en) + " " + en.fileName << std::endl;
        }
    }
    else if (this->options.g)
    {
        for (auto en : entries)
        {
            std::cout << fileInfoFunctions::fileInfo_toString_noOwner(en) + " " + en.fileName << std::endl;
        }
    }
    else
    {
        for (auto en : entries)
        {
            std::cout << en.fileName << "  ";
        }
        std::cout << std::endl;
    }
}