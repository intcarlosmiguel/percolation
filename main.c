#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include "bib/calc.h"
#include "bib/heap.h"
int N;

void invasive_percolation(int L,int seed,bool* existe,int G,int* cluster){
    struct minimalHeap Heap;
    Heap.size = 0;

    int site = (int)L/2+L/2*L,i,viz;

    srand(seed);

    bool* ocupado = calloc(N,sizeof(bool));
    bool* Lx = calloc(2,sizeof(bool));
    bool* Ly = calloc(2,sizeof(bool));
    bool* passou = calloc(N,sizeof(bool));
    bool* in_cluster = calloc(N,sizeof(bool));

    Heap.value = calloc(N,sizeof(double));
    for ( i = 0; i < L * L; i++) Heap.value[i] = (double)rand() / RAND_MAX;
    Heap.index = malloc(0*sizeof(int));

    double anterior = Heap.value[site];
    ocupado[site] = 1;
    FILE* arquivo;
    int cascate = 1;
    char filename[100];
    snprintf(filename, sizeof(filename), "./output/cascate/cascate_%d_%d.txt",L,G);
    if(G == 0)arquivo = fopen(filename, "a");
    in_cluster[site] = true;
    cluster[G]++;
    while (1) {
        
        for (i = 1; i < 5; i++) {
            viz = vizinho(site,i,L);
            //if(G==6) if(site == 76365) printf("Vizinho: %d\n",viz);
            if(viz == -1) continue;
            //if(G==1)printf("Existe: %d\n",existe[viz]);
            //if(G==1) if(existe[viz]) print_vector(Heap.index,Heap.size);
            if ((ocupado[viz] == 0) && (!passou[viz]) && (existe[viz])){
                Heap = add_heap(&Heap,viz);
                passou[viz] = true;
            }
        }
        site = Heap.index[0];
        in_cluster[site] = true;
        if(G == 0){
            if(Heap.value[site] <= anterior) cascate += 1;
            else{
                fprintf(arquivo,"%d\n",cascate);
                cascate = 1;
                anterior = Heap.value[site];
            }

        }
        heapify_down(&Heap);
        ocupado[site] = 1;
        cluster[G]++;
        if (X(site, L) == 0) break;
        if (X(site, L) == L - 1) break;
        if (Y(site, L) == 0) break;
        if (Y(site, L) == L - 1) break;
    }
    if(G == 0) fclose(arquivo);
    free(Heap.value);
    free(Heap.index);
    free(Lx);
    free(Ly);
    free(ocupado);
    free(passou);
    free(existe);
    if(G<500)invasive_percolation(L,seed+i,in_cluster,G+1,cluster);
    else free(in_cluster);
}

void main(int argc, char *argv[]) {

    int L = atoi(argv[1]);
    N = L*L;
    unsigned int seed = atoi(argv[2]);
    unsigned int redes = atoi(argv[3]);

    int i,j;
    int** cluster = (int**)malloc(redes*sizeof(int*));
    omp_set_num_threads(8);
    int count = 0;
    #pragma omp parallel for schedule(dynamic)
    for ( i = 0; i < redes; i++){
        cluster[i] = (int*)calloc(500,sizeof(int));
        bool* existe = calloc(N,sizeof(bool));
        for ( j = 0; j < N; j++) existe[j] = true;
        invasive_percolation(L,seed+i+600,existe,0,cluster[i]);
        count++;
        if(count%100 == 0) printf("Já foi: %d,%d",count,L);
    }
    
    FILE* file;
    char filename[100];
    sprintf(filename,"./output/df_%d.txt",L);
    file = fopen(filename, "w");
    double soma = 0;
    for ( i = 0; i < 500; i++){
        soma = 0;
        for ( j = 0; j < redes; j++) soma += cluster[j][i];
        fprintf(file,"%d %f\n",i,soma/redes);
    }
    fclose(file);
    free(cluster);
}