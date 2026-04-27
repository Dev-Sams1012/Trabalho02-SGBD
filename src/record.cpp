#include "record.hpp"

Record::Record(int Num, std::string texto) : linhaNum(Num), linhaTexto(texto) {}

Record Record::findInCSV(int key)
{
    std::ifstream file(CSV_FILE);

    if (!file.is_open()) //Verifica se o arquivo foi aberto corretamente
        return Record(-1, "");

    std::string line;

    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        size_t commaPos = line.find(','); //Encontra a posição da vírgula

        int currentKey = std::stoi(line.substr(0, commaPos));

        if (currentKey == key)
        {
            std::string texto = line.substr(commaPos + 1); //Extrai o texto após a vírgula
            if (!texto.empty() && texto.front() == '"')
                texto = texto.substr(1, texto.size() - 2);

            file.close();
            return Record(currentKey, texto);
        }
    }

    file.close();
    return Record(-1, "");
}