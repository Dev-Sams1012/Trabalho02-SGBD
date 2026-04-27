#include "hash_index.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

HashIndex::HashIndex(int initialDepth) : dir(initialDepth)
{
    std::ifstream test(DIRECTORY_FILE); //Verifica se o arquivo do diretório já existe para carregar ou criar um novo
    if (test.good())
    {
        test.close();
        dir.loadDirectory();
    }
    else
    {
        test.close();
        dir.initNew();

        for (int i = 0; i < dir.getSize(); ++i) //Cria os arquivos dos buckets iniciais
        {
            std::string name = dir.getBucketNameByIndex(i);
            Bucket b(name, initialDepth, true);
        }

        dir.saveDirectory();
    }
}

std::string HashIndex::newBucketName(int index) const //Gera um nome único para o novo bucket
{
    return "bucket_" + std::to_string(index) + "_d" +
           std::to_string(dir.getGlobalDepth()) + ".txt";
}

std::string HashIndex::splitBucket(const std::string &fullBucketName, int key) //Realiza a divisão do bucket
{
    Bucket old(fullBucketName, 0, false); 
    old.loadFromDisk(); //Carrega o bucket cheio para realizar a divisão

    int oldDepth = old.getLocalDepth();
    int newDepth = oldDepth + 1; //Aumenta a profundidade local do bucket

    int oldMask = (1 << oldDepth) - 1;
    int base = old.getKeys()[0] & oldMask; //Calcula a base para os índices dos buckets após a divisão

    int newBucketDirIndex = base | (1 << oldDepth);
    std::string newName = newBucketName(newBucketDirIndex); //Gera um nome para o novo bucket com base no índice calculado

    std::vector<int> allKeys = old.getKeys(); //Mantém uma cópia de todas as chaves do bucket antigo para redistribuição
    allKeys.push_back(key); 

    old.clearKeys();
    old.setLocalDepth(newDepth);

    {
        Bucket nb(newName, newDepth, true); //Cria o novo bucket para receber as chaves redistribuídas
    }

    {
        Bucket nb(newName, newDepth, false);
        nb.loadFromDisk();

        for (int k : allKeys) //Redistribui as chaves entre o bucket antigo e o novo com base no bit da profundidade local
        {
            if ((k >> oldDepth) & 1)
                nb.include(k);
            else
                old.include(k);
        }

        nb.saveToDisk();
    }

    old.saveToDisk();

    for (int i = 0; i < dir.getSize(); ++i) //Atualiza os ponteiros do diretório para os buckets divididos
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

//Implementação dos métodos de inserção, remoção e busca

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