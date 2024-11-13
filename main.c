#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include "bib/calc.h"
#include "bib/heap.h"
int N;

void invasive_percolation(int L,int seed,bool* existe,int G,double* cluster,bool plot,int*cluster_0){
    struct minimalHeap Heap;
    Heap.size = 0;

    int site = (int)L/2+L/2*L,i,viz;

    srand(seed);

    bool* ocupado = calloc(N,sizeof(bool));
    bool* in_cluster = calloc(N,sizeof(bool));

    Heap.value = calloc(N,sizeof(double));
    for ( i = 0; i < L * L; i++) Heap.value[i] = (double)rand() / RAND_MAX;
    Heap.index = malloc(0*sizeof(int));

    double anterior = Heap.value[site];
    ocupado[site] = true;

    FILE* arquivo;
    int cascate = 1;
    char filename[100];
    snprintf(filename, sizeof(filename), "./output/cascate/cascate_%d.txt",L);
    if(G == 0) arquivo = fopen(filename, "a");

    in_cluster[site] = true;
    int G_cluster = 1;
    while (1) {
        
        for (i = 1; i < 5; i++) {
            viz = vizinho(site,i,L);
            if(viz == -1) continue;
            if ((!ocupado[viz]) && (existe[viz])){
                Heap = add_heap(&Heap,viz);
                ocupado[viz] = true;
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
            *cluster_0 += 1;
        }
        heapify_down(&Heap);
        ocupado[site] = true;
        G_cluster++;
        if (X(site, L) == 0) break;
        if (X(site, L) == L - 1) break;
        if (Y(site, L) == 0) break;
        if (Y(site, L) == L - 1) break;
    }
    if(G == 0) fclose(arquivo);
    if(plot){
        FILE* file;
        sprintf(filename,"./output/plot/plot_%d_%d.txt",L,G);
        file = fopen(filename, "w");
        for ( i = 0; i < N; i++)if(in_cluster[i]) fprintf(file,"%d\n",i);
        fclose(file);
    }
    free(Heap.value);
    free(Heap.index);
    free(ocupado);
    free(existe);
    if(G<500){
        if(G != 0)cluster[G] += (double)G_cluster/ *cluster_0;
        //if(G != 0) printf("%f\n",(double)G_cluster/ *cluster_0);
        invasive_percolation(L,seed+i,in_cluster,G+1,cluster,plot,cluster_0);
    }
    else free(in_cluster);
}

void main(int argc, char *argv[]) {

    int L = atoi(argv[1]);
    N = L*L;
    unsigned int seed = atoi(argv[2]);
    unsigned int redes = atoi(argv[3]);

    int i,j;
    double** cluster = (double**)malloc(redes*sizeof(double*));
    int count = 0;
    //omp_set_num_threads(11);
    //#pragma omp parallel for
    int cluster_0;
    for ( i = 0; i < redes; i++){
        cluster[i] = (double*)calloc(500,sizeof(double));
        bool* existe = calloc(N,sizeof(bool));
        for ( j = 0; j < N; j++) existe[j] = true;
        cluster_0 = 0;
        invasive_percolation(L,seed+i+600,existe,0,cluster[i],i == 0,&cluster_0);
        count++;
        if(count%100 == 0) printf("Já foi: %d,%d\n",count,L);
    }
    
    FILE* file;
    char filename[100];
    sprintf(filename,"./output/G_%d.txt",L);
    file = fopen(filename, "w");
    double soma = 0;
    for ( i = 0; i < 500; i++){
        soma = 0;
        for ( j = 0; j < redes; j++)soma += cluster[j][i];
        fprintf(file,"%d %f\n",i,(double)soma/redes);
    }
    fclose(file);
    free(cluster);
}