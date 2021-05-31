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


//#include <iostream>


struct options{
    //directory is current dir by default
    const char *dir = ".";
    bool l = false;
    bool r = false;
}options;


struct fileInfo{
    std::string permissions;
    int link = 0;
    std::string owner;
    std::string groupOwner;
    __uint64_t size;
    std::string dateModified;
    std::string fileName;
};




void printArguments(int argc, const char *argv[]){
    printf("Number of arguments: %i\n", argc);
    for (int i = 0; i < argc; i++)
    {
        //std::cout << "arg nr " << argc << ": " << argv[i] << std::endl;
        printf("arg %i: ", i);
        printf("%s", argv[i]);
        printf("\n");
    }
    
}

std::string getFileCreationTime(const char* path) {
    struct stat attr;
    stat(path, &attr);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
    std::string temp = ctime(&attr.st_mtime);
    return temp;
}


std::string getFileOwner(const char* path) {
    struct stat attr;
    stat(path, &attr);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
    int userID = attr.st_uid;
    
    struct passwd *pw = getpwuid(attr.st_uid);
    // struct group  *gr = getgrgid(attr.st_gid);

    std::string temp = pw->pw_name;
    return temp;
}



std::string getFilePermissions(const char* path) {
    struct stat st;
    std::string modeval = "---------";

    if(stat(path, &st) == 0){
        mode_t perm = st.st_mode;
        modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
        modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
        modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
        modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
        modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
        modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
        modeval[6] = (perm & S_IROTH) ? 'r' : '-';
        modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
        modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
        return modeval;     
    }else{
        return "";
    } 

    return modeval;
}

std::string getFileGroup(const char* path) {
    struct stat attr;
    stat(path, &attr);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
    int userID = attr.st_uid;
    
    // struct passwd *pw = getpwuid(attr.st_uid);
    struct group  *gr = getgrgid(attr.st_gid);

    std::string temp = gr->gr_name;
    return temp;
}


//sets all except filename
fileInfo getFileInfo(const char *path, std::string filename){
    fileInfo info;
    struct stat attr;
    stat(path, &attr);
    info.dateModified = getFileCreationTime(path);
    info.fileName = filename;
    info.groupOwner = getFileGroup(path);
    info.owner = getFileOwner(path);
    info.permissions = getFilePermissions(path);
    info.size = attr.st_size;

    return info;
}


void parseArguments(int argc, const char *argv[]){
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


void _ls(const char *dir,int op_a,int op_l)
{
	//Here we will list the directory
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


int main(int argc, const char *argv[])
{
    parseArguments(argc, argv);
    const char* x = "/home/macianga/Desktop/polecenie_ls/ls.cpp";
    getFileCreationTime(x);

    _ls(options.dir,options.r, options.l);
    return 0;
}   
