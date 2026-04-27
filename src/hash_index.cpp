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

        for (int i = 0; i < dir.getSize(); ++i) //Cria os arquivos dos buckets iniciais, um por vez
        {
            std::string name = dir.getBucketNameByIndex(i);
            Bucket b(name, initialDepth, true);
        } //b é destruído a cada iteração, garantindo apenas um bucket em memória

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
    int oldDepth, newDepth, newBucketDirIndex;
    std::string newName;
    std::vector<int> keysForOld, keysForNew; //Listas de destino das chaves, calculadas antes de qualquer bucket novo entrar em memória

    { //Bloco 1: carrega o bucket antigo, separa as chaves e salva, sem criar o bucket novo
        Bucket old(fullBucketName, 0, false);
        old.loadFromDisk(); //Carrega o bucket cheio para realizar a divisão

        oldDepth = old.getLocalDepth();
        newDepth = oldDepth + 1; //Aumenta a profundidade local do bucket

        int oldMask = (1 << oldDepth) - 1;
        int base = old.getKeys()[0] & oldMask; //Calcula a base para os índices dos buckets após a divisão

        newBucketDirIndex = base | (1 << oldDepth);
        newName = newBucketName(newBucketDirIndex); //Gera um nome para o novo bucket com base no índice calculado

        std::vector<int> allKeys = old.getKeys(); //Mantém uma cópia de todas as chaves do bucket antigo para redistribuição
        allKeys.push_back(key);

        for (int k : allKeys) //Separa as chaves em duas listas com base no bit da profundidade local
        {
            if ((k >> oldDepth) & 1)
                keysForNew.push_back(k);
            else
                keysForOld.push_back(k);
        }

        old.clearKeys();
        old.setLocalDepth(newDepth);
        for (int k : keysForOld) //Reinsere no bucket antigo apenas as chaves que lhe pertencem
            old.include(k);
        old.saveToDisk();
    } //old é destruído aqui — fora de memória antes do novo bucket ser criado

    { //Bloco 2: cria e salva o novo bucket — o antigo já não está em memória
        Bucket nb(newName, newDepth, true); //Cria o novo bucket para receber as chaves redistribuídas
        for (int k : keysForNew) //Insere no novo bucket apenas as chaves que lhe pertencem
            nb.include(k);
        nb.saveToDisk();
    } //nb é destruído aqui

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
        } //b é destruído aqui em caso de sucesso

        int localDepth = b.getLocalDepth();
        b.saveToDisk(); //Salva e descarta o bucket antes de realizar o split

        if (localDepth == dir.getGlobalDepth()) //Se a profundidade local é igual à global, duplica o diretório
        {
            dir.duplicate();
            result.dirDuplicated = true;
            dir.saveDirectory();
        }

        splitBucket(bucketName, key); //Divide o bucket e redistribui as chaves
    }

    return result;
}

OpResult HashIndex::remove(int key)
{
    OpResult result{false, dir.getGlobalDepth(), 0, 0};

    std::string bucketName = dir.getBucketName(key);

    Bucket b(bucketName, 0, false);
    b.loadFromDisk();

    if (b.search(key)) //Remove a chave se encontrada no bucket
    {
        b.remove(key);
        result.qtdAffected = 1;
    }

    result.localDepth = b.getLocalDepth();
    result.globalDepth = dir.getGlobalDepth();

    b.saveToDisk();

    return result;
} //b é destruído aqui

OpResult HashIndex::search(int key)
{
    OpResult result{false, dir.getGlobalDepth(), 0, 0};

    std::string bucketName = dir.getBucketName(key);

    Bucket b(bucketName, 0, false);
    b.loadFromDisk();

    if (b.search(key)) //Se a chave está no bucket, busca o registro correspondente no CSV
    {
        result.qtdAffected = 1;

        Record rec = Record::findInCSV(key); //Lê o CSV uma linha por vez até encontrar o registro
        if (rec.getLinhaNum() != -1)
        {
            std::cout << "[BUS] " << rec.getLinhaNum()
                      << " | " << rec.getLinhaTexto() << std::endl;
        }
    }

    b.saveToDisk();

    return result;
} //b é destruído aqui

int HashIndex::getGlobalDepth() const
{
    return dir.getGlobalDepth();
}