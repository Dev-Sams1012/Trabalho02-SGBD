#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#define BUCKET_SIZE 5 //Tamanho máximo de chaves em um bucket
#define FOLDER "data/" //Pasta onde os arquivos dos buckets serão armazenados

class Bucket
{
private:
    std::vector<int> keys;
    std::string archiveName;
    int localDepth;

public:
    Bucket(std::string name, int depth, bool createNew = false);
    ~Bucket() = default;

    void saveToDisk();

    void loadFromDisk();

    bool include(int key);

    void remove(int key);

    bool search(int key);

    std::vector<int> getKeys() const { return keys; }
    std::string getArchiveName() const { return archiveName; }
    int getLocalDepth() const { return localDepth; }
    int getSize() const { return (int)keys.size(); }

    void setLocalDepth(int ld) { localDepth = ld; }
    void setArchiveName(std::string name) { archiveName = name; }
    void clearKeys() { keys.clear(); }
};

#endif