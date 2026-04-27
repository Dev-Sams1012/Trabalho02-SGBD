#ifndef HASH_INDEX_HPP
#define HASH_INDEX_HPP

#include "bucket.hpp"
#include "directory.hpp"
#include "record.hpp"
#include <string>
#include <vector>

struct OpResult //Estrutura para armazenar o resultado das operações
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

    std::string newBucketName(int index) const; //Gera um nome único para o novo bucket

    std::string splitBucket(const std::string &fullBucketName, int key); //Realiza a divisão do bucket

public:
    HashIndex(int initialDepth);

    OpResult insert(int key);

    OpResult remove(int key);

    OpResult search(int key);

    int getGlobalDepth() const;
};

#endif