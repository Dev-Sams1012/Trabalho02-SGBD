# Sistemas de Gerenciamento de Bancos de Dados - 2026.1

Este repositório contém o segundo trabalho de implementação da disciplina Sistemas de Gerenciamento de Bancos de Dados, ministrada pelo professor Javam Machado no semestre 2026.1.

## Descrição do Trabalho

O objetivo deste trabalho é implementar um **Índice Hash Extensível** para otimizar a recuperação de registros de um banco de dados armazenado em disco. O banco de dados contém trechos do livro *Dom Casmurro*, indexados pelo identificador único de cada linha (`LinhaNum`). O índice deve suportar as operações de busca por igualdade, inserção e remoção de entradas, respeitando a restrição de que apenas uma única página de dados pode estar em memória por vez.

## Estrutura do Projeto

```
├── data/
│   ├── bd-trab2 - dataset.csv     # Arquivo de dados com os trechos do livro
│   ├── in.txt                     # Arquivo de entrada com as operações do índice
│   ├── out.txt                    # Arquivo de saída gerado pelo programa
│   ├── diretorio.txt              # Arquivo persistido do diretório hash
│   └── bucket_*.txt               # Arquivos individuais de cada bucket
├── include/
│   ├── record.hpp                 # Definição da classe de registro (tupla)
│   ├── bucket.hpp                 # Definição da classe de bucket
│   ├── directory.hpp              # Definição da classe de diretório
│   └── hash_index.hpp             # Definição da classe principal do índice hash
└── src/
    ├── main.cpp                   # Leitura do in.txt e geração do out.txt
    ├── record.cpp                 # Busca de registros no arquivo CSV
    ├── bucket.cpp                 # Lógica de inserção, remoção e busca no bucket
    ├── directory.cpp              # Gestão do diretório e duplicação
    └── hash_index.cpp             # Operações de inserção, remoção, busca e split
```

## Funcionamento do Índice Hash Extensível

O índice é composto por dois componentes principais persistidos em disco:

- **Diretório:** Vetor de ponteiros para buckets, indexado pelos `PG` bits menos significativos da chave. Mantido inteiramente em memória durante a execução.
- **Buckets:** Arquivos individuais que armazenam até 5 entradas de dados cada. Apenas um bucket é carregado em memória por vez.

A função hash utilizada extrai os `PG` bits menos significativos do campo `LinhaNum` para determinar o índice de acesso ao diretório.

Quando um bucket está cheio ao se tentar inserir uma nova chave, ocorre um **split**: o bucket é dividido em dois, as chaves são redistribuídas e o diretório é atualizado. Caso a profundidade local do bucket iguale a profundidade global, o diretório é **duplicado** antes do split.

## Formato dos Arquivos de Entrada e Saída

**Entrada (`in.txt`):**
```
PG/<profundidade global inicial>
INC:<chave>
REM:<chave>
BUS=:<chave>
```

**Saída (`out.txt`):**
```
PG/<profundidade global inicial>
DUP DIR:/<profundidade global>,<profundidade local>   ← gerado antes de um INC que duplicou o diretório
INC:<chave>/<profundidade global>,<profundidade local>
REM:<chave>/<qtd removidas>,<profundidade global>,<profundidade local>
BUS:<chave>/<qtd encontradas>
P:/<profundidade global final>
```

## Como Compilar e Executar

### Pré-requisitos

- Compilador C++ com suporte a C++17
- CMake 3.10 ou superior

### Compilação

```bash
mkdir build
cd build
cmake ..
make
```

### Execução

Antes de executar, certifique-se de que o arquivo `data/in.txt` está configurado corretamente com as operações desejadas e que o arquivo `data/bd-trab2 - dataset.csv` está presente na pasta `data/`.

```bash
./HashIndex
```

O resultado será gerado em `data/out.txt`.

### Exemplo de `in.txt`

```
PG/2
INC:1
INC:5
INC:9
BUS=:5
REM:1
```

## Tecnologias Utilizadas

- **Linguagem de Programação:** C++
- **Sistema de Build:** CMake
- **Versionamento:** Git

## Autores

- Jonathan Duarte Uchôa
- Samuel Augusto de Abreu