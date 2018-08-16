#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <sstream>

class Data
{
public:
    Data();
    static void splitStr(const std::string &str, std::vector<std::string> &vec, const std::string &splitChar);
    static int scanDirslot(const std::string &path);
    static std::vector<std::string> getFileNames(const std::string &path);
    static std::string getFileName(const std::string &path);
    static std::string getFilePath();
    static bool is_dir_exist(const char *dir_path);
    static int getFileSize(const std::string &dir);
    static std::string currentFilePath(const std::string &path);

    static void cleanDirSlot(const std::string &path);
    static int getFileV(const std::string &path);
    static std::string intToString(int n);
    static int getIntLength(int x);
};

#endif // DATA_H

