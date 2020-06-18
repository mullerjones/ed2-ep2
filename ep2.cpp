#include <string>
#include "interface.cpp"
using namespace std;

int main(int argc, char** argv)
{
    int k;
    k = atoi(argv[1]);
    Grafo *grafo = new Grafo(k);

    return 0;
}