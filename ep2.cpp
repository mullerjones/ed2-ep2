#include <string>
#include <iostream>
#include "interface.hpp"
using namespace std;

int main(int argc, char** argv)
{
    int k;
    string nomeArquivo;
    k = atoi(argv[1]);
    nomeArquivo = argv[2];
    Grafo *grafo = new Grafo(k, nomeArquivo);
    int componentes = grafo->componentes();
    int vertices = grafo->vertices();
    int arestas = grafo->arestas();
    
    printf("teste/n");

    delete grafo;
    return 0;
}