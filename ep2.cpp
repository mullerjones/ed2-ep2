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
    string cachorro = "cachorro";
    string pato = "pato";
    string mato = "mato";
    string pata = "pata";
    string tapa = "tapa";
    if(grafo->dist(pato, mato) == 1) printf("pato e mato certo\n");
    if(grafo->dist(mato, pata) == 2) printf("mato e pata certo\n");
    if(grafo->dist(pato, cachorro) == -1) printf("pato e cachorro certo\n");
    if(grafo->dist(pata, tapa) == -1) printf("nao tem aresta onde era pra ter\n");
    printf("%d\n", grafo->dist(mato, tapa));
    
    printf("teste/n");

    return 0;
}