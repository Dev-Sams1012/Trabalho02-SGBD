#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#define FOLDER "data/"
#define DIRECTORY_FILE "data/diretorio.txt"

class Directory
{
private:
    int globalDepth;
    std::vector<std::string> pointers;

public:
    Directory(int pgInicial);
    ~Directory() = default;

    int getIndex(int key);

    std::string getBucketName(int key);

    void duplicate();

    void updatePointer(int index, std::string newBucketName);

    void saveDirectory();
    void loadDirectory();

    int getGlobalDepth() const { return globalDepth; }

    int getSize() const { return pointers.size(); }
};

#endif