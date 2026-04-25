#include "directory.hpp"
#include <iostream>
#include <cmath>

Directory::Directory(int pgInicial) : globalDepth(pgInicial) {}

void Directory::initNew()
{
    int numEntries = (int)std::pow(2, globalDepth);
    pointers.resize(numEntries);
    for (int i = 0; i < numEntries; ++i)
    {
        pointers[i] = "bucket_" + std::to_string(i) + ".txt";
    }
}

int Directory::getIndex(int key) const
{
    int mask = (1 << globalDepth) - 1;
    return key & mask;
}

std::string Directory::getBucketName(int key) const
{
    return pointers[getIndex(key)];
}

std::string Directory::getBucketNameByIndex(int index) const
{
    if (index >= 0 && index < (int)pointers.size())
        return pointers[index];
    return "";
}

void Directory::duplicate()
{
    int oldSize = (int)pointers.size();
    pointers.resize(oldSize * 2);

    for (int i = 0; i < oldSize; ++i)
    {
        pointers[i + oldSize] = pointers[i];
    }

    globalDepth++;
}

void Directory::updatePointer(int index, std::string newBucketName)
{
    if (index >= 0 && index < (int)pointers.size())
    {
        pointers[index] = newBucketName;
    }
}

void Directory::saveDirectory() const
{
    std::ofstream file(DIRECTORY_FILE);
    if (file.is_open())
    {
        file << globalDepth << "\n";
        for (const std::string &bucketFile : pointers)
        {
            file << bucketFile << "\n";
        }
        file.close();
    }
}

void Directory::loadDirectory()
{
    std::ifstream file(DIRECTORY_FILE);
    if (file.is_open())
    {
        std::string line;

        if (std::getline(file, line) && !line.empty())
        {
            globalDepth = std::stoi(line);
        }

        pointers.clear();
        while (std::getline(file, line) && !line.empty())
        {
            pointers.push_back(line);
        }
        file.close();
    }
}