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

    void initNew();

    int getIndex(int key) const;

    std::string getBucketName(int key) const;

    std::string getBucketNameByIndex(int index) const;

    void duplicate();

    void updatePointer(int index, std::string newBucketName);

    void saveDirectory() const;

    void loadDirectory();

    int getGlobalDepth() const { return globalDepth; }
    int getSize() const { return (int)pointers.size(); }
};

#endif