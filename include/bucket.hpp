#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#define BUCKET_SIZE 5
#define FOLDER "data/"

class Bucket
{
private:
    std::vector<int> keys;
    std::string archiveName;
    int localDepth;

public:
    Bucket(std::string name, int depth);
    ~Bucket() = default;

    void saveToDisk();
    void loadFromDisk();

    bool include(int key);
    void remove(int key);
    bool search(int key);

    std::vector<int> getKeys() const { return keys; }
    std::string getArchiveName() const { return archiveName; }
    int getLocalDepth() const { return localDepth; }

    void setLocalDepth(int ld) { localDepth = ld; }
    void setArchiveName(std::string name) { archiveName = name; }
};

#endif