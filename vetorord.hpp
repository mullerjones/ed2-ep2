#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "util.hpp"
using namespace std;

bool comparaValores(Valor a, Valor b)
{
    return strcasecmp(a.chave, b.chave) < 0;
}

class vetorOrd
{
public:
    vector<Valor> vetor;
    vetorOrd(string nomeArquivo, int k);
    ~vetorOrd();
    void insere(String chave, Integer valor);
    Integer devolve(String chave);
    void remove(String chave);
    int rank(String chave);
    String seleciona(int k);
    int quantosTem();

private:
    int find(String chave, int ini, int fin);
};

vetorOrd::vetorOrd(string nomeArquivo, int k)
{
    ifstream arqTexto;
    string palavra;
    String test = (String)malloc(40 * sizeof(char) + 5);
    Integer aux = nullptr;
    arqTexto.open(nomeArquivo);
    while (arqTexto >> palavra)
    {
        if (palavra.length() >= k)
        {
            strcpy(test, palavra.c_str());
            aux = devolve(test);
            if (aux == nullptr)
            {
                int *um = new int;
                *um = 1;
                insere(test, um);
            }
            else
            {
                (*aux)++;
            }
        }
    }
    free(test);
    test = nullptr;
    aux = nullptr;
    arqTexto.close();
}

vetorOrd::~vetorOrd()
{
    for (std::size_t i = 0; i < vetor.size(); ++i)
    {
        free(vetor[i].chave);
        delete vetor[i].valor;
    }
    vetor.clear();
}

void vetorOrd::insere(String chave, Integer valor)
{
    Valor aux;
    aux.chave = (String)malloc(40 * sizeof(char) + 5);
    strcpy(aux.chave, chave);
    aux.valor = valor;
    if (vetor.size() == 0)
    {
        vetor.push_back(aux);
    }
    else
    {
        auto itPos = upper_bound(vetor.begin(), vetor.end(), aux, &comparaValores);
        vetor.insert(itPos, aux);
    }
    return;
}

Integer vetorOrd::devolve(String chave)
{
    int index = find(chave, 0, vetor.size() - 1);
    if (index == -1)
        return nullptr;
    return vetor.at(index).valor;
}

void vetorOrd::remove(String chave)
{
    int index = find(chave, 0, vetor.size() - 1);
    delete vetor.at(index).valor;
    free(vetor.at(index).chave);
    vetor.erase(vetor.begin() + index - 1);
}

int vetorOrd::rank(String chave)
{
    int rank = find(chave, 0, vetor.size() - 1);
    return rank;
}

String vetorOrd::seleciona(int k)
{
    return vetor.at(k).chave;
}

int vetorOrd::find(String chave, int ini, int fin)
{
    if (vetor.size() == 0)
        return -1;
    int i = (ini + fin) / 2;
    int valor = strcasecmp(vetor.at(i).chave, chave);
    if (ini == fin && valor != 0)
    {
        return -1;
    }

    if (valor == 0)
        return i;

    switch (valor / abs(valor))
    {
    case -1:
        return find(chave, i + 1, fin);
    case 1:
        return find(chave, ini, i);
    }

    return 0;
}

int vetorOrd::quantosTem()
{
    return vetor.size();
}