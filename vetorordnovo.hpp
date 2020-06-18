#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

//#include "util.hpp"
using namespace std;

bool comparaChaves(string a, string b)
{
    return strcasecmp(a, b) < 0;
}

class vetorOrd
{
public:
    vector<string> vetor;
    vetorOrd(string nomeArquivo, int k);
    ~vetorOrd();
    void insere(string palavra);
    int devolve(string chave);
    void remove(string chave);
    string seleciona(int k);
    int quantosTem();

private:
    int find(string chave, int ini, int fin);
};

vetorOrd::vetorOrd(string nomeArquivo, int k)
{
    ifstream arqTexto;
    string palavra;
    arqTexto.open(nomeArquivo);
    while (arqTexto >> palavra)
    {
        if (palavra.length() >= k)
        {
            aux = devolve(palavra);
            if (aux == -1)
            {
                insere(palavra);
            }
        }
    }
    arqTexto.close();
}

vetorOrd::~vetorOrd()
{
    vetor.clear();
}

void vetorOrd::insere(string chave)
{
    if (vetor.size() == 0)
    {
        vetor.push_back(aux);
    }
    else
    {
        auto itPos = upper_bound(vetor.begin(), vetor.end(), &comparaChaves);
        vetor.insert(itPos, chave);
    }
    return;
}

int vetorOrd::devolve(string chave)
{
    int index = find(chave, 0, vetor.size() - 1);
    return index;
}

void vetorOrd::remove(string chave)
{
    int index = find(chave, 0, vetor.size() - 1);
    vetor.erase(vetor.begin() + index - 1);
}

string vetorOrd::seleciona(int k)
{
    return vetor.at(k);
}

int vetorOrd::find(string chave, int ini, int fin)
{
    if (vetor.size() == 0)
        return -1;
    int i = (ini + fin) / 2;
    int valor = strcasecmp(vetor.at(i), chave);
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