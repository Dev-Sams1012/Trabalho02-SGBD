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

void Bucket::saveToDisk() //Persiste no disco o conteúdo atualizado do bucket
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

void Bucket::loadFromDisk() //Carrega do disco o conteúdo do bucket para a memória
{
    keys.clear(); //Limpa as chaves antes da nova leitura
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

bool Bucket::include(int key) //Insere uma chave no bucket
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

void Bucket::remove(int key) //Remove uma chave do bucket
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

bool Bucket::search(int key) //Busca uma chave no bucket
{
    for (int k : keys)
    {
        if (k == key)
            return true;
    }
    return false;
}