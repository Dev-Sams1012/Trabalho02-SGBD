#ifndef HASH_INDEX_HPP
#define HASH_INDEX_HPP

#include "bucket.hpp"
#include "directory.hpp"
#include "record.hpp"
#include <string>
#include <vector>

struct OpResult
{
    bool dirDuplicated;
    int globalDepth;
    int localDepth;
    int qtdAffected;
};

class HashIndex
{
private:
    Directory dir;

    std::string newBucketName(int index) const;

    std::string splitBucket(const std::string &fullBucketName, int key);

public:
    HashIndex(int initialDepth);

    OpResult insert(int key);

    OpResult remove(int key);

    OpResult search(int key);

    int getGlobalDepth() const;
};

#endif