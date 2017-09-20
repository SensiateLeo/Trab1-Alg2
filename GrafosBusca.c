/* Descricao das funcoes declaradas me Grafos - busca.h */
#include <stdio.h>
#include <stdlib.h>
#include "GrafosBusca.h"

//#include "Grafos - Matriz adjacencia.h"

#define INF 2500000

int Floydinho(tgrafo *grafo){
   int V = grafo->num_vertices;
   int dist[V][V];
   int i,j, k;
    /* Preenche a matriz com infinito*/
   for(i = 0; i < V; i++){
        for(j = 0; j < V; j++){
            dist[i][j] = INF;
        }
   }
   /* Preenche a diagonal principal com 0's*/
   for(i = 0; i < V; i++){
        for(j = 0; j < V; j++){
            if(i == j){
                dist[i][j] = 0;
            }
        }
   }

   tapontador p;

    /*Coloca os pesos iniciais na matriz*/
   for(i = 0; i < V; i++){
        p = grafo->vet[i]->prox;
        while(p != NULL){
            if(i != p->id && p->peso != INF){
                dist[i][p->id] = p->peso;
            }
            p = p->prox;
        }
   }
   /*Atualiza os menores caminhos*/
   for(i = 0; i < V; i++){
        for(j = 0; j < V; j++){
            for(k = 0; k < V; k++){
                if(i != j && j != k){
                    if(dist[j][i] + dist[i][k] < dist[j][k])
                    dist[j][k] = dist[j][i] + dist[i][k];
                }
            }
        }
   }

   tapontador_vertice v;

   /*Multiplica a matriz pelo numero de egressos
   para levarmos em conta o menor deslocamento de pessoas
   (pessoas * deslocamento)*/
   for(i = 0; i < V; i++){
        for(j = 0; j < V; j++){
            v = grafo->vet[i];
            dist[i][j] = dist[i][j]*v->egressos;
        }
   }

   /* Cria um vetor de excentricidades*/
   int exc[V];
   /*Encontra o custo m�ximo de cada coluna da matriz*/
   int maior = 0;
   for(i = 0; i < V; i++){
        for(j = 0; j < V; j++){
            if(i != j && dist[j][i] != INF && maior < dist[j][i])
                maior = dist[j][i];
        }
        exc[i] = maior;
        maior = 0;
   }

   maior = exc[0];
   int verticecentral = 0;
   int entradas;
   tapontador q;

   /*Escolhe o v�rtice central
   O de menos excentricidade entre as maiores */
   for(i = 0; i < V; i++){
        v = grafo->vet[i];
        for(j = 0; j < V; j++){
            q = grafo->vet[j]->prox;

            entradas = 0;
            while(q != NULL){
                if(i != j && q->id == v->id){
                    entradas = 1;
                }
                q = q->prox;
            }
            if(entradas == 1 && exc[v->id] < maior){
                maior = exc[v->id];
                verticecentral = v->id;
            }
        }
   }

    return verticecentral;
}

int quantidadeDeVertices(tgrafo *grafo){
    return grafo->num_vertices;
}

void betweenness(int ant[][MAXNUMVERTICES], int numv){
    int cidadeAtual, i, j;
    int valorBetweenness[numv];      //cada lugar do vetor armazena o valor de betweenness para cada vertice
    int seguidorDeCaminho;           //a funcao dessa variavel esta explicada abaixo

    for (j = 0; j < numv; j++){
        valorBetweenness[j] = 0;                        //iniciliza o vetor nulo
    }

    for(cidadeAtual = 0; cidadeAtual < numv; cidadeAtual++){  // cidade em que est� sendo calculado o betweenness
        printf("cidadeAtual:%d \n", cidadeAtual);
        for(i = 0; i < numv; i++){                            //linha da matriz: cidade de origem
            if(i != cidadeAtual){                             //a cidade que estamos analisando n�o pode ser origem
                printf("Origem:%d ", i);
                for(j = 0; j < numv; j++){                    //cidade de destino
                    if(j > i && j != cidadeAtual){            /*a primeira compara��o serve como otimiza��o para grafos n�o
                                                              orientados (para ida e volta, o menor caminho � o mesmo) e a
                                                              segunda determina que a cidade analisada n�o pode ser destino*/
                        printf(" Destino:%d", j);
                        seguidorDeCaminho = ant[i][j];           /*seguidorDeCaminho serve para armazenar os v�rtices antecessores
                                                                permitindo assim "caminhar" pelo vetor do destino at� a origem*/
                        while(seguidorDeCaminho != -1){
                            if(seguidorDeCaminho == cidadeAtual){       /*se o seguidorDeCaminho for igual a cidade analisada
                                                                        ela est� no meio de um dos menores caminhos no grafo,
                                                                        ent�o temos que incrementar o valor de betweenness da cidade
                                                                        e j� podemos analisar outros caminhos*/
                                valorBetweenness[cidadeAtual]++;
                                printf("++");
                                seguidorDeCaminho = -1;
                            }
                            else{
                                seguidorDeCaminho = ant[i][seguidorDeCaminho];  //se n�o encontrar a cidade que queremos, volta no caminho
                            }
                        }
                    }
                }
                printf(" Betweenness:%d \n", valorBetweenness[cidadeAtual]);
            }
        }
        printf("\n");
    }

    for(i = 0; i < numv; i++){
        printf("\nBetweenness da cidade %d: %d", i, valorBetweenness[i]);
    }
}

void Dijkstra(tgrafo *grafo){
    /*Pega o numero de vertices
    cria um vetor para armazenar a distancia para cada vertice
    e outro vetor para armazenar os antecessores (menor caminho at� todos) */
    int numv = grafo->num_vertices;
    float dist[numv][numv];
    int ant[numv][numv];
    int i, j, marcacao[numv][numv];
    tapontador arestaAtual;
    tapontador_vertice verticeAtual;


    /*Preenche o vetor de dist�ncias com infinito e 0 para a raiz inderida na funcao*/
    for (i = 0; i < numv; i++){
        for(j = 0; j < numv; j++){
            if(i == j){
            dist[i][j] = 0;
        }
        else{
            dist[i][j] = INF;
        }
        marcacao[i][j] = BRANCO;
        ant[i][j] = -1;
        }
    }

    float menor;
    int contadorMax, cidadeAtual;

    //for para passar por todos os vertices
    for(cidadeAtual = 0; cidadeAtual < numv; cidadeAtual++){
        verticeAtual = grafo->vet[cidadeAtual];
        arestaAtual = grafo->vet[cidadeAtual]->prox;

        //Dijkstra em si
        for(contadorMax = 0; contadorMax < numv; contadorMax++){
            while(arestaAtual != NULL){
                if((arestaAtual->peso + dist[cidadeAtual][verticeAtual->id] < dist[cidadeAtual][arestaAtual->id]) && (marcacao[cidadeAtual][arestaAtual->id] == BRANCO)){
                    dist[cidadeAtual][arestaAtual->id] = arestaAtual->peso + dist[cidadeAtual][verticeAtual->id];
                    ant[cidadeAtual][arestaAtual->id] = verticeAtual->id;
                }
                arestaAtual = arestaAtual->prox;
            }
            marcacao[cidadeAtual][verticeAtual->id] = PRETO;

            menor = INF;
            for (i = 0; i < numv; i++){
                if ((dist[cidadeAtual][i] < menor) && (marcacao[cidadeAtual][i] != PRETO)){
                    menor = dist[cidadeAtual][i];
                    verticeAtual = grafo->vet[i];
                    arestaAtual = verticeAtual->prox;
                }
            }

            if(menor == INF){
                break;
            }
        }


        /*Printa o vetor de menores distancias*/

        printf("Vertice de origem : %d", cidadeAtual);
        printf("\nMenores distancias:");
        for(j = 0; j < numv; j++){
            if(dist[cidadeAtual][j] == INF){
                printf("  INF");
            }
            else
                printf(" %.2f", dist[cidadeAtual][j]);
            }
        printf("\n");


        /*Printa o vetor de menores caminhos (antecessores)*/
        printf("\nAntecessores (caminho): ");
        for(j = 0; j < numv; j++){
            printf(" %d", ant[cidadeAtual][j]);
        }
        printf("\n\n");
    }

    //Come�o do Betweenness
    int valorBetweenness[numv];      //cada lugar do vetor armazena o valor de betweenness para cada vertice
    int seguidorDeCaminho;           //a funcao dessa variavel esta explicada abaixo

    for (j = 0; j < numv; j++){
        valorBetweenness[j] = 0;                        //iniciliza o vetor nulo
    }

    for(cidadeAtual = 0; cidadeAtual < numv; cidadeAtual++){  // cidade em que est� sendo calculado o betweenness
        printf("cidadeAtual:%d \n", cidadeAtual);
        for(i = 0; i < numv; i++){                            //linha da matriz: cidade de origem
            if(i != cidadeAtual){                             //a cidade que estamos analisando n�o pode ser origem
                printf("Origem:%d ", i);
                for(j = 0; j < numv; j++){                    //cidade de destino
                    if(j > i && j != cidadeAtual){            /*a primeira compara��o serve como otimiza��o para grafos n�o
                                                              orientados (para ida e volta, o menor caminho � o mesmo) e a
                                                              segunda determina que a cidade analisada n�o pode ser destino*/
                        printf(" Destino:%d", j);
                        seguidorDeCaminho = ant[i][j];           /*seguidorDeCaminho serve para armazenar os v�rtices antecessores
                                                                permitindo assim "caminhar" pelo vetor do destino at� a origem*/
                        while(seguidorDeCaminho != -1){
                            if(seguidorDeCaminho == cidadeAtual){       /*se o seguidorDeCaminho for igual a cidade analisada
                                                                        ela est� no meio de um dos menores caminhos no grafo,
                                                                        ent�o temos que incrementar o valor de betweenness da cidade
                                                                        e j� podemos analisar outros caminhos*/
                                valorBetweenness[cidadeAtual]++;
                                printf("++");
                                seguidorDeCaminho = -1;
                            }
                            else{
                                seguidorDeCaminho = ant[i][seguidorDeCaminho];  //se n�o encontrar a cidade que queremos, volta no caminho
                            }
                        }
                    }
                }
                printf(" Betweenness:%d \n", valorBetweenness[cidadeAtual]);
            }
        }
        printf("\n");
    }

    for(i = 0; i < numv; i++){
        printf("\nBetweenness da cidade %d: %d", i, valorBetweenness[i]);
    }

}




