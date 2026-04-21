#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>

class Record
{
private:
    int linhaNum;
    std::string linhaTexto;

public:
    Record(int Num, std::string texto);
    ~Record() = default;
};

#endif