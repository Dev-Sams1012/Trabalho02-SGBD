#include "hash_index.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#define INPUT_FILE "data/in.txt"
#define OUTPUT_FILE "data/out.txt"

int main()
{
    std::filesystem::create_directories("data"); //Garante que a pasta "data" exista para armazenar os arquivos do diretório e buckets

    std::ifstream inFile(INPUT_FILE); //Abre o arquivo de entrada para leitura
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

    HashIndex idx(initialDepth); //Inicializa o índice hash com a profundidade global inicial lida do arquivo

    std::ofstream outFile(OUTPUT_FILE); //Abre o arquivo de saída para escrita dos resultados
    if (!outFile.is_open())
    {
        std::cerr << "Erro: nao foi possivel criar " << OUTPUT_FILE << std::endl;
        return 1;
    }

    outFile << firstLine << "\n";

    std::string line;
    while (std::getline(inFile, line)) //Lê cada linha do arquivo de entrada para processar as operações de inserção, remoção e busca
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