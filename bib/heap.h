#pragma once
#include <math.h>
struct minimalHeap{
    int* index;
    double* value;
    int size;
};

int pai(int i){
    if(i%2 == 0) return (int)i/2 - 1;
    else return (int)(i+1)/2 - 1;
}
int filhos(int i,int aux){
    if(aux == 0) return (i+1)*2;
    else return (int)(i+1)*2-1;
}

int camada_heap(int indice) {
    // Adicionamos 1 ao índice para adaptar à fórmula, conforme explicação.
    return (int)floor(log2(indice + 1));
}
void heapify_down(struct minimalHeap* Heap){
    int i = 0;
    int s = Heap->index[0];
    swap(&Heap->index[i], &Heap->index[Heap->size-1]);
    Heap->index = realloc(Heap->index,(Heap->size-1)*sizeof(int));
    Heap->size--;
    int index = 0;
    int menor_valor = (Heap->value[Heap->index[filhos(0,0)]] < Heap->value[Heap->index[filhos(0,1)]])? 0:1;
    if(Heap->size > 2){
        
        while (Heap->value[Heap->index[filhos(index,menor_valor)]] < Heap->value[Heap->index[index]]){
            swap(&Heap->index[index], &Heap->index[filhos(index,menor_valor)]);
            index = filhos(index,menor_valor);
            menor_valor = (Heap->value[filhos(index,0)] < Heap->value[filhos(index,1)])? 0:1;
            if(camada_heap(filhos(index,menor_valor)) >= camada_heap(Heap->size)) break;
        }

    }
}

struct minimalHeap add_heap(struct minimalHeap* Heap,int i){
    // Aloca um novo espaço para o novo elemento no heap
    Heap->size++;
    //if(i == 81000) print_vector(Heap->index,Heap->size);
    //if(i == 81000) printf("Dentro da função: ");
    Heap->index = (int*)realloc(Heap->index, Heap->size*sizeof(int));
    Heap->index[Heap->size-1] = i;

    if(Heap->size > 1){
        
        int index = Heap->size-1;
        while((Heap->value[Heap->index[index]] < Heap->value[Heap->index[pai(index)]]) && (index != 0)){
            swap(&Heap->index[pai(index)], &Heap->index[index]);
            index = pai(index);
        }
    }
    return *Heap;
}