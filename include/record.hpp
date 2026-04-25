#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <fstream>
#include <sstream>

#define CSV_FILE "data/bd-trab2 - dataset.csv"

class Record
{
private:
    int linhaNum;
    std::string linhaTexto;

public:
    Record(int Num, std::string texto);
    ~Record() = default;

    int getLinhaNum() const { return linhaNum; }
    std::string getLinhaTexto() const { return linhaTexto; }

    static Record findInCSV(int key);
};

#endif