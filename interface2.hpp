#include <string>
#include <string.h>
#include <queue>
#include "vetorord.hpp"
using namespace std;

bool checaSemLetraI(string primeira, string segunda, int i);
bool checaTrocaLetras(string a, string b, int tamanho);
bool checaTrocarUmaLetra(string a, string b, int tamanho);

//k definido como no enunciado será passado como argumento de linha de comando
class Grafo
{

private:
    vetorOrd *tabela;
    bool **matrizAdj;
    int numPalavras;
    int numArestas;
    int parametro;
    int c[10000];
    bool checaAresta(int i, int j);
    void marcaVisitado(int i, int valor);
    bool checaFilhos(int pai, int procurado, queue<int> *fila, bool *jaFoi);
    bool procuraNosFilhos(int atual, int procurado, bool *visitados);
    bool buscaLargura(int pai, int procurado, queue<int> *fila, bool *jaFoi, bool* caminho);

public:
    Grafo(int k, string nomeArquivo)
    {
        /* Inicializa um grafo com parâmetro k */
        /*Le arquivo e monta tabela de simbolos*/
        int i, j;
        parametro = k;
        //string nomeArquivo;
        //cout << "Insira nome do arquivo base: ";
        //cin >> nomeArquivo;
        tabela = new vetorOrd(nomeArquivo, k);
        /*Conta numero de palavras e aloca matriz de adjacencias*/
        numPalavras = tabela->quantosTem();
        numArestas = 0;
        matrizAdj = (bool **)malloc(numPalavras * sizeof(bool *));
        for (i = 0; i < numPalavras; i++)
        {
            matrizAdj[i] = (bool *)malloc(numPalavras * sizeof(bool));
            for (j = 0; j < numPalavras; j++)
            {
                matrizAdj[i][j] = false;
            }
        }
        /*Inicializa arestas*/
        for (i = 0; i < numPalavras; i++)
        {
            for (j = i + 1; j < numPalavras; j++)
            {
                matrizAdj[i][j] = checaAresta(i, j);
                matrizAdj[j][i] = matrizAdj[i][j];
                if (matrizAdj[i][j])
                    numArestas++;
            }
        }

        for (i = 0; i < 10000; i++)
        {
            c[i] = -1;
        }
    }

    ~Grafo()
    {
        for (int i = 0; i < numPalavras; i++)
        {
            delete matrizAdj[i];
        }
        delete matrizAdj;
        delete tabela;
    }

    int insere(string palavra)
    {
        /* Insere a palavra e retorna o número de arestas adicionados ao grafo,
        retorna -1 se a palavra já está no grafo ou tem tamanho menor que k*/
        if (palavra.length() < parametro)
            return -1;
        String test = (String)malloc(40 * sizeof(char) + 5);
        strcpy(test, palavra.c_str());
        if (*tabela->devolve(test) == -1)
            return -1;

        int numArestasAntes = numArestas;
        int *aux = new int;
        *aux = 1;
        tabela->insere(test, aux);
        numPalavras++;
        int novoNum = 0;
        for (int i = 0; i < numPalavras; i++)
        {
            for (int j = i + 1; j < numPalavras; j++)
            {
                matrizAdj[i][j] = checaAresta(i, j);
                matrizAdj[j][i] = matrizAdj[i][j];
                if (matrizAdj[i][j])
                    novoNum++;
            }
        }
        numArestas = novoNum;
        return novoNum - numArestasAntes;
    }

    int vertices()
    {
        /* Retorna o número de vértices do grafo*/
        return numPalavras;
    }

    int arestas()
    {
        /* Retorna o número de arestas do grafo*/
        int total = 0;
        for (int i = 0; i < numPalavras; i++)
        {
            for (int j = i + 1; j < numPalavras; j++)
            {
                if (matrizAdj[i][j])
                    total++;
            }
        }
        return total;
    }

    int componentes()
    {
        /* Retorna o número de componentes do grafo */
        int i, j;
        int num = 0;
        for (i = 0; i < numPalavras; i++)
        {
            c[i] = -1;
        }
        for (i = 0; i < numPalavras; i++)
        {
            if (c[i] == -1)
            {
                num++;
                marcaVisitado(i, num);
            }
        }
        return num;
    }

    bool conexo()
    {
        /* Retorna se o grafo é ou não conexo */
        return componentes() == 1;
    }

    int tamComp(string palavra)
    {
        /* Retorna o tamanha da componente conexa onde está a palavra
        ou -1 caso ela não se encontre no grafo */
        int valor;
        String test = (String)malloc(40 * sizeof(char) + 5);
        strcpy(test, palavra.c_str());
        valor = *tabela->devolve(test);
        if (valor == -1)
            return -1;
        else
        {
            /*calcula tamanho da comp*/
            /*TODO*/
        }
        free(test);
    }

    int dist(string a, string b)
    {
        /* Retorna a menor distância entre as palavras a e b ou -1
        caso elas estejam desconexas ou não estejam no grafo */
        int i1, i2;
        String aux1 = (String)malloc(40 * sizeof(char) + 5);
        strcpy(aux1, a.c_str());
        i1 = tabela->rank(aux1);
        if (i1 == -1)
            return -1;

        String aux2 = (String)malloc(40 * sizeof(char) + 5);
        strcpy(aux2, b.c_str());
        i2 = tabela->rank(aux2);
        if (i2 == -1)
            return -1;
        if (matrizAdj[i1][i2])
            return 1;

        free(aux1);
        free(aux2);
        /*A distancia sera calculada realizando uma busca em largura*/
        /*Utilizarei uma fila para marcar o proximo indice a ser procurado*/
        int dist = 1;
        queue<int> *fila = new queue<int>;
        bool acabou = false;
        bool jaFoi[numPalavras];
        for (int i = 0; i < numPalavras; i++)
        {
            jaFoi[i] = false;
        }
        int iatual = i1;
        checaFilhos(iatual, i2, fila, jaFoi);
        while (!acabou)
        {
            fila->push(-1);
            dist++;
            while ((iatual = fila->front()) != -1)
            {
                fila->pop();
                if (checaFilhos(iatual, i2, fila, jaFoi))
                    acabou = true;
            }
            fila->pop();
            if (fila->empty())
            {
                acabou = true;
                dist = -1;
            }
        }

        while (!fila->empty())
            fila->pop();
        delete fila;

        return dist;
    }

    bool emCiclo(string a)
    {
        /* Retorna verdadeiro casa a palavra esteja em algum ciclo,
        falso caso contrário */
        String aux = (String)malloc(40 * sizeof(char) + 5);
        strcpy(aux, a.c_str());
        int index = tabela->rank(aux);
        free(aux);

        bool jaVisitou[numPalavras];
        for (int i = 0; i < numPalavras; i++)
        {
            jaVisitou[i] = false;
        }
        bool achou = false;
        for (int i = 0; !achou && i < numPalavras; i++)
        {
            if (i != index && matrizAdj[index][i])
            {
                jaVisitou[i] = true;
                for (int j = 0; !achou && j < numPalavras; j++)
                {
                    if (matrizAdj[i][j])
                    {
                        achou = procuraNosFilhos(j, index, jaVisitou);
                    }
                }
                if (!achou)
                {
                    for (int j = 0; j < numPalavras; j++)
                    {
                        jaVisitou[j] = false;
                    }
                }
            }
        }

        return achou;
    }

    bool emCiclo(string a, string b)
    {
        /* Retorna verdadeiro casa exista um ciclo que contenha ambas as palavras,
        falso caso contrário */
        /*
        if (dist(a, b) == -1)
            return false;

        int i1, i2;
        String aux1 = (String)malloc(40 * sizeof(char) + 5);
        strcpy(aux1, a.c_str());
        i1 = tabela->rank(aux1);
        if (i1 == -1)
            return false;

        String aux2 = (String)malloc(40 * sizeof(char) + 5);
        strcpy(aux2, b.c_str());
        i2 = tabela->rank(aux2);
        if (i2 == -1)
            return false;

        free(aux1);
        free(aux2);

        bool jaFoi[numPalavras];
        bool caminho[numPalavras];
        for (int i = 0; i < numPalavras; i++)
        {
            jaFoi[i] = false;
            caminho[i] = false;
        }

        bool acabou = false;
        queue<int> *fila = new queue<int>;
        int iatual = i1;
        buscaLargura(iatual, i2, fila, jaFoi);
        while (!acabou)
        {
            fila->push(-1);
            while ((iatual = fila->front()) != -1)
            {
                fila->pop();
                if (buscaLargura(iatual, i2, fila, jaFoi, caminho))
                    acabou = true;
            }
            fila->pop();
            if (fila->empty())
            {
                acabou = true;
            }
        }

        while (!fila->empty())
            fila->pop();
            */
    }


};

bool Grafo::checaAresta(int i, int j)
{
    /*Checa cada possibilidade de criterio para existencia de aresta*/
    string a = tabela->seleciona(i);
    string b = tabela->seleciona(j);
    bool aux = false;
    aux = checaTrocarUmaLetra(a, b, a.length());
    if (!aux)
    {
        if (a.length() - b.length() == 1)
        {
            for (int i = 0; !aux && i < a.length(); i++)
            {
                aux = checaSemLetraI(b, a, i);
            }
        }
        else if (b.length() - a.length() == 1)
        {
            for (int i = 0; !aux && i < b.length(); i++)
            {
                aux = checaSemLetraI(a, b, i);
            }
        }
    }
    if (!aux)
        aux = checaTrocaLetras(a, b, a.length());

    return aux;
}

void Grafo::marcaVisitado(int i, int valor)
{
    c[i] = valor;
    for (int j = 0; j < numPalavras; j++)
    {
        if (matrizAdj[i][j] && c[j] == -1)
            marcaVisitado(j, valor);
    }
}

bool Grafo::checaFilhos(int pai, int procurado, queue<int> *fila, bool *jaFoi)
{
    if (matrizAdj[pai][procurado])
        return true;
    jaFoi[pai] = true;
    for (int i = 0; i < numPalavras; i++)
    {
        if (matrizAdj[pai][i] && !jaFoi[i])
            fila->push(i);
    }
    return false;
}

bool Grafo::procuraNosFilhos(int atual, int procurado, bool *visitados)
{
    if (visitados[atual])
        return false;
    if (matrizAdj[atual][procurado])
        return true;
    visitados[atual] = true;
    bool achou = false;
    for (int i = 0; i < numPalavras && !achou; i++)
    {
        if (!visitados[i] && matrizAdj[atual][i])
            achou = procuraNosFilhos(i, procurado, visitados);
    }

    return achou;
}

bool Grafo::buscaLargura(int pai, int procurado, queue<int> *fila, bool *jaFoi, bool* caminho)
{
    if (matrizAdj[pai][procurado])
    {
        jaFoi[pai] = true;
        return true;
    }
    jaFoi[pai] = true;
    for (int i = 0; i < numPalavras; i++)
    {
        if (matrizAdj[pai][i] && !jaFoi[i])
            fila->push(i);
    }
    return false;
}

bool checaSemLetraI(string primeira, string segunda, int i)
{
    /*Checa se a string primeira eh igual à string segunda caso removamos da segunda a letra de indice i*/
    /*A funcao so eh chamada se a segunda string tiver tamanho igual ao da primeira +1, logo isso nao eh conferido*/
    string aux = segunda;
    aux.erase(aux.begin() + i);
    return primeira == aux;
}

bool checaTrocaLetras(string a, string b, int tamanho)
{
    /*Checa se a string a eh igual à b caso troquemos 2 letras de lugar */
    if (a.length() != b.length())
        return false;

    /*Como essa propriedade vale de a para b sse valer de b para a,     */
    /*podemos fixar uma das strings e variar apenas a outra             */
    /*Copia as strings para um char* */
    char a2[tamanho];
    char b2[tamanho];
    a.copy(a2, tamanho);
    b.copy(b2, tamanho);
    char aux;
    int ajuda;
    /*Testa cada possibilidade*/
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = i + 1; j < tamanho; j++)
        {
            /*troca 2 letras de lugar*/
            aux = a2[i];
            a2[i] = a2[j];
            a2[j] = aux;
            /*Se for igual, existe essa aresta por esse criterio*/
            ajuda = 0;
            for (int k = 0; k < tamanho; k++)
            {
                if (a2[k] != b2[k])
                    ajuda = -1;
            }
            if (ajuda == 0)
                return true;
            /*Se nao, desfaz troca para proxima comparacao*/
            else
            {
                aux = a2[i];
                a2[i] = a2[j];
                a2[j] = aux;
            }
        }
    }
    /*Se nenhuma iteracao achou uma combinacao que confirmasse, nao ha essa aresta por esse criterio*/
    return false;
}

bool checaTrocarUmaLetra(string a, string b, int tamanho)
{
    /*Checa se a string a eh igual à b caso troquemos uma letra*/
    if (a.length() != b.length())
        return false;

    /*Podemos conferir essa possibilidade verificando se as strings sao   */
    /*iguais com excessao de apenas um caracter na mesma posicao em ambas */
    /*Copia as strings para um char* */
    char a2[tamanho];
    char b2[tamanho];
    a.copy(a2, tamanho);
    b.copy(b2, tamanho);
    bool achou = false;
    bool esseAindaSim = true;
    for (int i = 0; !achou && i < tamanho; i++)
    { /*i eh o indice a ser desconsiderado*/
        for (int j = 0; j < tamanho && esseAindaSim; j++)
        { /*itera pelas palavras comparando cada letras menos i*/
            /*Se acha alguma letra diferente, para a busca*/
            if (i != j && a2[j] != b2[j])
                esseAindaSim = false;
        }
        /*Se sai do loop porque acabou a palavra, a condicao eh valida*/
        if (esseAindaSim)
            achou = true;
        else
        {
            esseAindaSim = true;
        }
    }
    return achou;
}
