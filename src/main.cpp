#include "hash_index.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#define INPUT_FILE "data/in.txt"
#define OUTPUT_FILE "data/out.txt"

int main()
{
    std::filesystem::create_directories("data");

    std::ifstream inFile(INPUT_FILE);
    if (!inFile.is_open())
    {
        std::cerr << "Erro: nao foi possivel abrir " << INPUT_FILE << std::endl;
        return 1;
    }

    std::string firstLine;
    std::getline(inFile, firstLine);

    if (firstLine.rfind("PG/", 0) != 0)
    {
        std::cerr << "Erro: primeira linha invalida (esperado PG/<n>): "
                  << firstLine << std::endl;
        return 1;
    }

    int initialDepth = std::stoi(firstLine.substr(3));

    HashIndex idx(initialDepth);

    std::ofstream outFile(OUTPUT_FILE);
    if (!outFile.is_open())
    {
        std::cerr << "Erro: nao foi possivel criar " << OUTPUT_FILE << std::endl;
        return 1;
    }

    outFile << firstLine << "\n";

    std::string line;
    while (std::getline(inFile, line))
    {
        if (line.empty())
            continue;

        if (line.rfind("INC:", 0) == 0)
        {
            int key = std::stoi(line.substr(4));
            OpResult res = idx.insert(key);

            if (res.dirDuplicated)
            {
                outFile << "DUP_DIR:/" << res.globalDepth << ","
                        << res.localDepth << "\n";
            }

            outFile << "INC:" << key << "/"
                    << res.globalDepth << ","
                    << res.localDepth << "\n";
        }
        else if (line.rfind("REM:", 0) == 0)
        {
            int key = std::stoi(line.substr(4));
            OpResult res = idx.remove(key);

            outFile << "REM:" << key << "/"
                    << res.qtdAffected << ","
                    << res.globalDepth << ","
                    << res.localDepth << "\n";
        }
        else if (line.rfind("BUS=:", 0) == 0)
        {
            int key = std::stoi(line.substr(5));
            OpResult res = idx.search(key);

            outFile << "BUS:" << key << "/"
                    << res.qtdAffected << "\n";
        }
        else
        {
            std::cerr << "Aviso: linha ignorada (formato desconhecido): "
                      << line << std::endl;
        }
    }

    outFile << "P:/" << idx.getGlobalDepth() << "\n";

    inFile.close();
    outFile.close();

    std::cout << "Concluido. Resultado salvo em " << OUTPUT_FILE << std::endl;
    return 0;
}