#Relatorio EP2
##Alexandre Muller Jones - nUSP 8038149

##Introdução
Neste EP, implementei uma estrutura de um grafo conforme instruções do enunciado. Minha implementação utilizou algumas funções auxiliares adicionais, além dos métodos que foram fornecidos no arquivo de interface dado pelos monitores. Este arquivo sofreu algumas mudanças, principalmente em torna-lo um arquivo .hpp ao invés de cpp, de modo a tornar a compilação do arquivo de testes mais facil e facilitar, também, sua inclusão em quaisquer arquivos que os monitores usarão para a correção.

A seguir, descreverei com mais detalhes a implementação do grafo como um todo e dos métodos em especifico.

##Estrutura da implementação
Para a implementação do grafo, utilizei um vetor de strings e uma matriz de booleanos que representa as adjacencias. Como se trata de um grafo não direcionado, temos uma matriz simétrica com diagonal principal 0. Me aproveitei deste fato para acelerar a montagem da matriz, visto que apenas precisava olhar diretamente o triângulo superior e então refleti-lo para o inferior de modo a garantir que a matriz matrizAdj fosse simetrica, ou seja, que `matrizAdj[i][j] == matrizAdj[j][i]` seria verdadeiro para quaisquer i e j.

Escolhi utilizar um vetor ordenado ao invés de desordenado. Essa escolha torna a execução da tarefa de inserção mais demorada, já que é preciso refazer a matriz de adjacencias a cada chamada. Entretanto, há um ganho significativo em ações que envolvam uma palavra específica, já que a busca da palavra no vetor torna-se muito mais eficiente por se tornar uma busca binária.

Essa decisão esta muito relacionada ao intuito que o grafo teria em um contexto maior. Caso seu uso fosse relacionado primariamente a um texto estático (ou seja, se desejasse observar as propriedades de ciclos, tamanho de componentes etc. para um texto em específico), a implementação que escolhi seria a mais ideal, dado que não seriam feitas inserções custosas e se teria um ganho considerável em cada observação que fosse feita sobre ele. Entretanto, caso inserções fossem frequentes e observações apenas mais eventuais, uma implementação em vetor desordenado (que, por consequência, permitiria que a matriz de adjacencias não fosse sempre refeita e as novas adições do grafo fossem adicionadas às bordas dela) seria mais eficiente. Reconheço que, no contexto deste EP, a diferença é pequena e estamos lidando apenas com uma prova de conceito. Entretanto, acho importante, exatamente por se tratar de uma oportunidade de aprendizado, termos esse tipo de distinção em mente e termos consciencia dos efeitos que escolhas de implementação como essa terão no produto final.

Outra otimização que fiz foi criar os membros numPalavras e numArestas. numPalavras é criado junto ao grafo e atualizado sempre que é feita a inserção de uma nova palavra. O mesmo vale para numArestas, que é verificado na criação do grafo e sempre que a matriz de adjacencias é atualizada, de modo a estar sempre condizente com ela. Dessa forma, não é preciso repetir a iteração pelo vetor ou pela matriz de adjacências para obter a saida dos métodos arestas() e vertices(), o que diminui muito o tempo dessas consultas, especialmente para grafos grandes.

A ultima mudança foi incluir o nome do arquivo a ser lido no construtor do Grafo, o que facilitou bastante a iteração com arquivos diferentes. Caso seja preferível para correção, mantive o prompt que pede o nome do arquivo durante a execução no código em um comentário, de modo que pode ser descomentado e essa inclusão na interface pode ser apagada para que a interface volte exatamente à fornecida.

###Funções de arestas
Conforme o enunciado, temos 3 critérios que devem ser checados para conferir a existência ou não de uma aresta entre 2 palavras. Portanto, implementei 3 funções auxiliares que checam cada um desses critérios.

####checaSemLetraI()
Esta função checa se uma string *primeira* é igual à string *segunda* caso a letra de indice i seja removida da segunda. Isso é feito criando uma cópia da segunda (de forma a poder apagar uma letra sem interferir na original), se apagando essa letra e conferindo a igualdade. A função foi implementada desta forma para facilitar seu uso na função principal relacionada ao grafo, a checaAresta(), que será descrita mais abaixo.

####checaTrocaLetras()
Esta função confere o critério da troca entre 2 letras (como `pata` e `tapa`). Ela confere se as 2 palavras tem mesmo tamanho (dado que, caso contrário, nunca haverá aresta por esse critério) e, em caso positivo, confere cada possibilidade de troca, retornando positivo caso encontre algum caso verdadeiro. Vale ressaltar que essa propriedade pode ser checada em qualquer direção já que, por exemplo, trocar a primeira e a terceira letras de `pata` resulta em `tapa` e vice versa. Assim, podemos fixar uma das strings e variar a outra, conferindo se há alguma diferença. Se encontrada a alteração que iguala as palavras, retorna positivo e, caso contrário, desfaz a troca, repetindo o processo até conferir todas as possibilidades, o que indica que o critério não vale.

####checaTrocarUmaLetra()
Este critério busca palavras que difiram em apenas uma letra (como `nato` e `nata`). Podemos verificar se isso é verdade fazendo outra pergunta que é mais fácil de computar e implica necessariamente que o critério é verdadeiro: existe j tal que `a[i] == b[i]` para qualquer i != j? Iniciamos a checagem conferindo se as strings tem mesmo tamanho e, em seguida, conferimos cada indíce entre 0 e o tamanho das strings em busca deste j. Caso o encontremos, retorna positivo.

##checaAresta()
Este método do grafo usa as 3 funções auxiliares acima para conferir os critérios para qualquer par de strings, indicados pelos seus indices i e j no vetor. Checa-se inicialmente o critério checaTrocarUmaLetra, já que é o mais rápido de executar. Em seguida, se checa se a diferença entre o tamanho das strings é exatamente 1, já que nesse caso o checaSemLetraI pode ser verdadeiro. Se sim, essa função é chamada para cada indice possível. Por ultimo, é chamada a função mais lenta, a checaTrocaLetras. Dessa forma, minimizamos o tempo de execução no geral já que os critérios mais lentos só são chamados em ultimo caso.

##Métodos do grafo
A seguir, falarei sobre cada método do grafo em específico e informações relevantes sobre sua implementação.

###insere()
A inserção é bastante simples, envolvendo apenas checar se a palavra tem tamanho suficiente para ser incluida no grafo, inseri-la na posição correta no vetor e refazer a matriz de adjacencias. Junto com isso, contamos o novo número de arestas para salvar em numArestas e retornamos a diferença entre esse valor e o valor anterior.

###vertices() e arestas()
Conforme descrito acima, esses métodos apenas acessam os membros privados numPalavras e numArestas.

###componentes()
Essa função utiliza um membro privado, o vetor de inteiros c, para marcar a qual componente a palavra de indice i pertence, o que é acessado por `c[i]`. Escolhi implementar desta forma me baseando no pseudo-código do professor [Paulo Feofiloff](https://www.ime.usp.br/~pf/algoritmos_em_grafos/aulas/componentes.html), especialmente considerando que isso facilitaria outras implementações posteriores. O vetor c é inicializado com valor -1, que representa que aquela palavra não foi visitada ainda, e então utilizo a função auxiliar marcaVisitado() para percorrer o grafo marcando cada vértice na mesma componente da inicial com um valor. O valor começa em 1 e é incrementado para cada componente posterior, de forma que posso usá-lo como a própria saida.

####marcaVisitado()
Esta função auxiliar percorre uma componente em profundidade de forma recursiva, marcando cada vértice de uma componente com o valor passado e chamando a si mesma para cada vértice adjacente ao inicial.

###conexo()
Este método deve retornar positivo caso o grafo seja conexo. Caso ele seja conexo, todos os vértices estarão em uma mesma componente, ou seja, basta apenas chamar componentes() e verificar que sua saída é 1.

###tamComp()
Este método retorna o número de vértices na mesma componente que a palavra dada. Verifico isso a partir do vetor c que a função componentes() cria. Primeiro, verifico se c já tem os rotulos de cada vértice (conferindo se `c[0] != 1`). Caso contrario, chamo esta função de modo a garantir que c esta com os valores corretos e então conto quantas vezes o valor de `c[i]`, onde i é o indice da palavra procurada no vetor de palavras, aparece em c, contando a si mesmo.

###dist()
Este método calcula a distancia entre duas strings no grafo. Após fazer checagens basicas (conferir que ambas as palavras estão no grafo e que não são adjacentes), utilizo uma busca em largura para calcular a menor distancia entre as 2 palavras. Utilizei uma fila implementada com a biblioteca queue, um vetor de booleanos que marca quais vertices ja foram visitados e a função auxiliar checaFilhos() que coloca na fila todos os vértices adjacentes ao atual que ainda não foram visitados. O algoritmo segue da seguinte forma:
1. Coloca na fila todos os vertices de distancia 1
2. Coloca -1 na fila
3. Esvazia a fila, conferindo para cada vertice se o procurado é adjacente e, caso contrario, colocando cada adjacente ainda não visitado na fila
4. Ao encontrar -1 na fila, incrementa a distancia e coloca -1 na fila novamente
5. Repete os itens 3 e 4 até que se encontre o procurado ou todos os membros sejam visitados

Caso encontremos a palavra procurada, retornamos a distancia. Caso contrario, ou seja, a fila esteja vazia e todos os vertices visitados, retornamos -1.

###emCiclo(string a)
Este método confere se determinada palavra esta num ciclo. Aqui, não nos importamos com o tamanho deste ciclo, apenas com sua existência. Utilizei uma função auxiliar procuraNosFilhos() que, recursivamente, realiza uma busca em profundidade pelo grafo, marcando cada vertice visitado num vetor de booleanos conforme metodos anteriores até que todos os vértices adjacentes a ele tenham sido visitados. Chamo esta função, com o vetor de booleanos jaFoi sendo resetado a cada iteração, a partir de cada vértice adjacente ao procurado. Em outras palavras, o código tenta procurar um caminho até o vertice inicial a partir de cada um dos adjacentes a ele até que todos esses se esgotem ou o caminho seja encontrado. Caso encontre um caminho, sabemos que o vértice esta num ciclo, ja que foi possivel chegar nele a partir de um vértice adjacente por outro caminho.

###emCiclo(string a, string b)
Este overload do método anterior realiza algo similar, porém verificando se 2 vertices distintos estão em um mesmo ciclo. Antes de procurar este ciclo, faço algumas checagens mais rapidas que excluiríam essa possibilidade, especificamente se ambas as palavras estão no grafo e, caso o vetor c ja tenha sido inicializado, se estão na mesma componente. Essas informações são necessárias mas não suficientes, o que as torna boas formas de excluir casos extremos.

Passado isso, o algoritmo principal utiliza 2 vetores de booleanos, jaFoi e caminho, e uma função auxiliar procuraNosFilhosGuardando(), que realiza uma busca em largura partindo de um vértice a procura do outro, desde que não passando por qualquer vértice já no caminho. Assim, o vetor jaFoi é utilizado para encontrar o primeiro caminho de a à b, e então todos os vértices deste caminho são marcados no vetor caminho. Depois disso, realizo o processo inverso, procurando um caminho de b à a. Caso a busca retorne positivo, sabemos que há há 2 caminhos diferentes de a até b, o que significa que eles estão em um mesmo ciclo.

A busca por cada um destes caminhos é feita em largura ao invés de profundidade, como foi na implementação anterior, de forma a garantir que cada caminho seja o menor possível e, portanto, que um não será desnecessariamente grande a ponto de passar por vértices desnecessários que formariam o caminho de volta.

###~Grafo()
O destrutor do grafo deleta a matriz de adjacencias e esvazia o vetor de vértices, liberando toda a memória alocada.

##Análise
Realizei testes da interface usando primariamente 2 arquivos. O primeiro, palavras.txt, consiste de uma quantidade pequena de palavras escolhidas manualmente de forma que eu soubesse exatamente o comportamento esperado e pudesse testar o funcionamento da interface. O segundo, minusc.txt, consiste de uma lista das 1000 palavras mais comuns na lingua inglesa, retirado [deste](https://www.ef.com/wwen/english-resources/english-vocabulary/top-1000-words/) site, que coloquei em uma única linha e todas em letra minúscula (com o auxílio de um pequeno script em *Python*). Escolhi palavras em inglês ao invés de português de modo que não tivesse que me preocupar com caractéres diferentes (como ã ou é) ou outros bugs desse tipo. Curiosamente, essas 1000 palavras formam um grafo muito pouco denso, com muito menos arestas do que se imaginaria, e bastante desconexo, com uma quantidade bem grande de componentes.

##Conclusões
A implementação de certas partes da interface foi mais desafiadora do que eu imaginei. Implementar buscas em profundidade não apresentou muita dificuldade, dado como essas buscas se prestam à recursão, mas buscas em largura me trouxeram muitas dúvidas. Como marcar quais vértices eu ja tinha visitado? Como me assegurar que estava procurando o menor caminho? Como manipular corretamente essas informações? Todos estes questionamentos surgiram durante a implementação, mas acredito que saio deste lado com mais conhecimento e mais segurança do meu entendimento sobre grafos.
