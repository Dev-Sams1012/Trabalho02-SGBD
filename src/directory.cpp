#include "directory.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

Directory::Directory(int pgInicial) : globalDepth(pgInicial)
{
    int numEntries = std::pow(2, globalDepth);
    pointers.resize(numEntries);

    for (int i = 0; i < numEntries; ++i)
    {
        pointers[i] = "bucket_" + std::to_string(i) + ".txt";
    }
}

int Directory::getIndex(int key)
{
    int mask = (1 << globalDepth) - 1;
    return key & mask;
}

std::string Directory::getBucketName(int key)
{
    int index = getIndex(key);
    return pointers[index];
}

void Directory::duplicate()
{
    int oldSize = pointers.size();
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

void Directory::saveDirectory()
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