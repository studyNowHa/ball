#include "data.h"
#include <iostream>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils/Log.h"

Data::Data()
{

}

/**
 * Split std::string to array
 * @param str -- the std::string need to be splited.
 * @param vec -- std::vector use to store splited std::string item.
 * @param splitChar -- the split character
 */
void Data::splitStr(const std::string &str, std::vector<std::string> &vec, const std::string &splitChar)
{
    std::string::size_type pos = str.find(splitChar);
    std::string::size_type pos1 = 0;
    while(std::string::npos !=  pos)
    {
        vec.push_back(str.substr(pos1, pos -pos1));
        pos1 = pos + splitChar.size();
        pos = str.find(splitChar, pos1);
    }
    if(pos1 != str.length())
    {
        vec.push_back(str.substr(pos1));
    }
}

int Data::scanDirslot(const std::string &path)
{
    std::string str = Data::currentFilePath(path);
    if(Data::is_dir_exist(str.c_str()))
    {
        int count = Data::getFileSize(str);
        if(count)
        {
            return count;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

}

std::vector<std::string> Data::getFileNames(const std::string &path)
{
    DIR *dp;
    struct dirent *dirp;
    std::vector<std::string> file_names;
    dp = opendir(path.c_str());
    while((dirp = readdir(dp))!=NULL)
    {
        if((strcmp(dirp->d_name,".")==0) || (strcmp(dirp->d_name,"..")==0))
            continue;
        file_names.push_back(dirp->d_name);
    }
    return file_names;
}

std::string Data::getFileName(const std::string &path)
{

}

std::string Data::getFilePath()
{
    // change by jiwei for board root path
    std::string currentPath = "/mnt/extsd/simple_socket_server_";
    return currentPath;   
}

bool Data::is_dir_exist(const char *dir_path)
{

    if (dir_path == NULL)
        return false;
    if(opendir(dir_path) == NULL)
    {
        return false;
    }
	else
    {
        return true;
    }
}

int Data::getFileSize(const std::string &dir)
{
    std::vector<std::string> file_names = Data::getFileNames(dir);
    return file_names.size();
}

std::string Data::currentFilePath(const std::string &path)
{
    std::string str = Data::getFilePath();
    //cout << "s std::string: " << str << endl;
    str = str.substr(0,str.find("simple_socket_server_", 0));
    //cout << "scan std::string: " << str << endl;
    str = str.append(path);
   // cout << "final std::string: " << str << endl;
    if(!Data::is_dir_exist(str.c_str()))
    {
	    mkdir(str.c_str(), S_IRWXU);
        //std::string cmd = std::string("mkdir ") + str;
        //system(cmd.c_str);
    }
    return str;
}

void Data::cleanDirSlot(const std::string &path)
{
        DIR *pDir = NULL;
        struct dirent *dmsg;
        char szFileName[128];
        char szFolderName[128];

        strcpy(szFolderName, path.c_str());
        strcat(szFolderName, "/%s");
        if ((pDir = opendir(path.c_str())) != NULL)
        {
           // __android_log_print(ANDROID_LOG_INFO, COM_HSDI_LOG_TITLE, "open dir success");
            // 遍历目录并删除文件
            while ((dmsg = readdir(pDir)) != NULL)
            {
               // __android_log_print(ANDROID_LOG_INFO, COM_HSDI_LOG_TITLE, "do while %s-->%s", szFileName);
                if (strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)
                {
                    sprintf(szFileName, szFolderName, dmsg->d_name);
                   // __android_log_print(ANDROID_LOG_INFO, COM_HSDI_LOG_TITLE, "do remove %s", szFileName);
                    std::string tmp = szFileName;
                    //如果是文件夹，名称中不包含"."
                    if (tmp.find(".") == -1){
                        cleanDirSlot(szFileName);
                    }
                    remove(szFileName);
                }
            }
        }

        if (pDir != NULL)
        {
            closedir(pDir);
        }
}

int Data::getFileV(const std::string &path)
{
    FILE *fp;
    int fd;
    struct stat buf;
    fp = fopen(path.c_str(),"r");
    fd = fileno(fp);
    fstat(fd, &buf);
    int size = buf.st_size;
    return size;
}

std::string Data::intToString(int n)
{
	std::stringstream stream;
	stream << n;
	return stream.str();
}

int Data::getIntLength(int x)
{
	int len=0;
	while(x)
	{
		x/=10;
		len++;
	}
	return len;
}

