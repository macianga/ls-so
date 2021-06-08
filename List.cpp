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


#include <iostream>

void List::parseArguments(int argc, const char *argv[]){
    for (int i = 0; i < argc; i++)
    {
        //check if argument is an option       
        if(argv[i][0] == '-'){
            if(argv[i][1] == 'l'){
                options.l = true;
            }
            if(argv[i][1] == 'r'){
                options.r = true;
            }
            if(argv[i][1] == 'o'){
                //TODO more arguments
                //options.l = true;
            }
        }
        if(argv[i][0] == '/'){
            options.dir = argv[i];
        }
    }   
}

void List::printArguments(int argc, const char *argv[]){
    printf("Number of arguments: %i\n", argc);
    for (int i = 0; i < argc; i++)
    {
        //std::cout << "arg nr " << argc << ": " << argv[i] << std::endl;
        printf("arg %i: ", i);
        printf("%s", argv[i]);
        printf("\n");
    }
    
}

void List::_ls()
{
	//Here we will list the directory
    const char * dir = this->options.dir;
    int op_a = this->options.r;
    int op_l = this->options.l;
	struct dirent *d;
	DIR *dh = opendir(dir);
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
    
    std::vector<std::string> entries;

	while ((d = readdir(dh)) != NULL)
	{
		//If hidden files are found we continue
		if (!op_a && d->d_name[0] == '.')
			continue;
            
		printf("%s  ", d->d_name);
        entries.push_back(d->d_name);
		if(op_l) printf("\n");
	}
	if(!op_l)
	printf("\n");
}