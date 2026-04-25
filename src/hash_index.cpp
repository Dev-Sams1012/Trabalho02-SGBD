#include "hash_index.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

HashIndex::HashIndex(int initialDepth) : dir(initialDepth)
{
    std::ifstream test(DIRECTORY_FILE);
    if (test.good())
    {
        test.close();
        dir.loadDirectory();
    }
    else
    {
        test.close();
        dir.initNew();

        for (int i = 0; i < dir.getSize(); ++i)
        {
            std::string name = dir.getBucketNameByIndex(i);
            Bucket b(name, initialDepth, true);
        }

        dir.saveDirectory();
    }
}

std::string HashIndex::newBucketName(int index) const
{
    return "bucket_" + std::to_string(index) + "_d" +
           std::to_string(dir.getGlobalDepth()) + ".txt";
}

std::string HashIndex::splitBucket(const std::string &fullBucketName, int key)
{
    Bucket old(fullBucketName, 0, false);
    old.loadFromDisk();

    int oldDepth = old.getLocalDepth();
    int newDepth = oldDepth + 1;

    int oldMask = (1 << oldDepth) - 1;
    int base = old.getKeys()[0] & oldMask;

    int newBucketDirIndex = base | (1 << oldDepth);
    std::string newName = newBucketName(newBucketDirIndex);

    std::vector<int> allKeys = old.getKeys();
    allKeys.push_back(key);

    old.clearKeys();
    old.setLocalDepth(newDepth);

    {
        Bucket nb(newName, newDepth, true);
    }

    {
        Bucket nb(newName, newDepth, false);
        nb.loadFromDisk();

        for (int k : allKeys)
        {
            if ((k >> oldDepth) & 1)
                nb.include(k);
            else
                old.include(k);
        }

        nb.saveToDisk();
    }

    old.saveToDisk();

    for (int i = 0; i < dir.getSize(); ++i)
    {
        if (dir.getBucketNameByIndex(i) == fullBucketName)
        {
            if ((i >> oldDepth) & 1)
                dir.updatePointer(i, newName);
        }
    }

    dir.saveDirectory();

    return dir.getBucketName(key);
}

OpResult HashIndex::insert(int key)
{
    OpResult result{false, 0, 0, 0};

    while (true)
    {
        std::string bucketName = dir.getBucketName(key);

        Bucket b(bucketName, 0, false);
        b.loadFromDisk();

        if (b.include(key))
        {
            result.globalDepth = dir.getGlobalDepth();
            result.localDepth = b.getLocalDepth();
            b.saveToDisk();
            break;
        }

        int localDepth = b.getLocalDepth();
        b.saveToDisk();

        if (localDepth == dir.getGlobalDepth())
        {
            dir.duplicate();
            result.dirDuplicated = true;
            dir.saveDirectory();
        }

        splitBucket(bucketName, key);
    }

    return result;
}

OpResult HashIndex::remove(int key)
{
    OpResult result{false, dir.getGlobalDepth(), 0, 0};

    std::string bucketName = dir.getBucketName(key);

    Bucket b(bucketName, 0, false);
    b.loadFromDisk();

    if (b.search(key))
    {
        b.remove(key);
        result.qtdAffected = 1;
    }

    result.localDepth = b.getLocalDepth();
    result.globalDepth = dir.getGlobalDepth();

    b.saveToDisk();

    return result;
}

OpResult HashIndex::search(int key)
{
    OpResult result{false, dir.getGlobalDepth(), 0, 0};

    std::string bucketName = dir.getBucketName(key);

    Bucket b(bucketName, 0, false);
    b.loadFromDisk();

    if (b.search(key))
    {
        result.qtdAffected = 1;

        Record rec = Record::findInCSV(key);
        if (rec.getLinhaNum() != -1)
        {
            std::cout << "[BUS] " << rec.getLinhaNum()
                      << " | " << rec.getLinhaTexto() << std::endl;
        }
    }

    b.saveToDisk();

    return result;
}

int HashIndex::getGlobalDepth() const
{
    return dir.getGlobalDepth();
}