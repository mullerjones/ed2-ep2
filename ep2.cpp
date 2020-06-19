#include <string>
#include <iostream>
#include "interface2.hpp"
using namespace std;

int main(int argc, char** argv)
{
    int k;
    string nomeArquivo;
    k = atoi(argv[1]);
    nomeArquivo = argv[2];
    Grafo *grafo = new Grafo(k, nomeArquivo);
    int componentes = grafo->componentes();
    printf("teste/n");

    return 0;
}