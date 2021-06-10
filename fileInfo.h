#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <algorithm>

struct fileInfo
{
    std::string permissions;
    int link = 0;
    std::string owner;
    std::string groupOwner;
    __uint64_t size;
    std::string dateModified;
    std::string fileName;
};

namespace fileInfoFunctions
{

    std::string getFileCreationTime(const char *path)
    {
        struct stat attr;
        stat(path, &attr);
        //printf("Last modified time: %s", ctime(&attr.st_mtime));
        std::string temp = ctime(&attr.st_mtime);
        return temp.erase(temp.length() - 1);
        if (!temp.empty() && temp[temp.length() - 1] == '\n')
        {
            temp.erase(temp.length() - 1);
        }
        return temp;
    }

    std::string getFileOwner(const char *path)
    {
        struct stat attr;
        stat(path, &attr);
        //printf("Last modified time: %s", ctime(&attr.st_mtime));
        int userID = attr.st_uid;

        struct passwd *pw = getpwuid(attr.st_uid);
        // struct group  *gr = getgrgid(attr.st_gid);

        std::string temp = pw->pw_name;
        return temp;
    }

    std::string getFilePermissions(const char *path)
    {
        struct stat st;
        std::string modeval = "---------";

        if (stat(path, &st) == 0)
        {
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
        }
        else
        {
            return "";
        }

        return modeval;
    }

    std::string getFileGroup(const char *path)
    {
        struct stat attr;
        stat(path, &attr);
        //printf("Last modified time: %s", ctime(&attr.st_mtime));
        int userID = attr.st_uid;

        // struct passwd *pw = getpwuid(attr.st_uid);
        struct group *gr = getgrgid(attr.st_gid);

        std::string temp = gr->gr_name;
        return temp;
    }

    fileInfo getFileInfo(const char *path, std::string filename)
    {
        std::string real_path = path;
        if(real_path[0] == '.'){
            real_path += "/";
        }
        real_path += filename;

        fileInfo info;
        struct stat attr;
        //printf("%s \n", real_path.c_str());
        stat(real_path.c_str(), &attr);
        info.dateModified = getFileCreationTime(real_path.c_str());
        info.fileName = filename;
        info.groupOwner = getFileGroup(real_path.c_str());
        info.owner = getFileOwner(real_path.c_str());
        info.permissions = getFilePermissions(real_path.c_str());
        info.size = attr.st_size;

        return info;
    }

    long int getFileSize(char file_name[])
    {
        // opening the file in read mode
        FILE *fp = fopen(file_name, "r");

        // checking if the file exist or not
        if (fp == NULL)
        {
            printf("File Not Found!\n");
            return -1;
        }

        fseek(fp, 0L, SEEK_END);

        // calculating the size of the file
        long int res = ftell(fp);

        // closing the file
        fclose(fp);
        return res;
    }

        std::string fileInfo_toString(fileInfo info)
        {
            std::string str = "";
            str += info.permissions;
            str += " ";
            str += std::to_string(info.link);
            str += " ";
            str += info.owner;
            str += " ";
            str += info.groupOwner;
            str += " ";
            str += std::to_string(info.size);
            str += "   ";
            str += info.dateModified;
            str += " ";

            return str;
        }

        std::string fileInfo_toString_noOwner(fileInfo info)
        {
            std::string str = "";
            str += info.permissions;
            str += " ";
            str += std::to_string(info.link);
            str += " ";
            str += std::to_string(info.size);
            str += "   ";
            str += info.dateModified;
            str += " ";

            return str;
        }
    }