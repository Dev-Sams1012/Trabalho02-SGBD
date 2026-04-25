#include "bucket.hpp"

Bucket::Bucket(std::string name, int depth, bool createNew)
    : archiveName(name), localDepth(depth)
{
    if (createNew)
    {
        std::ofstream file(FOLDER + archiveName);
        if (file.is_open())
        {
            file << localDepth << std::endl;
            file.close();
        }
    }
}

void Bucket::saveToDisk()
{
    std::ofstream file(FOLDER + archiveName);
    if (file.is_open())
    {
        file << localDepth << std::endl;
        for (int k : keys)
        {
            file << k << std::endl;
        }
        file.close();
    }
}

void Bucket::loadFromDisk()
{
    keys.clear();
    std::ifstream file(FOLDER + archiveName);
    if (file.is_open())
    {
        std::string line;
        if (std::getline(file, line) && !line.empty())
        {
            localDepth = std::stoi(line);
        }

        while (std::getline(file, line) && !line.empty())
        {
            keys.push_back(std::stoi(line));
        }
        file.close();
    }
}

bool Bucket::include(int key)
{
    if (search(key))
        return true;

    if (keys.size() >= BUCKET_SIZE)
    {
        return false;
    }

    keys.push_back(key);
    return true;
}

void Bucket::remove(int key)
{
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i] == key)
        {
            keys.erase(keys.begin() + i);
            break;
        }
    }
}

bool Bucket::search(int key)
{
    for (int k : keys)
    {
        if (k == key)
            return true;
    }
    return false;
}