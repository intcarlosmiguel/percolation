#pragma once
void print_vector(int* vetor,int N){
    for (int i = 0; i <N; i++){
        if(i!=N-1) printf("%d ",vetor[i]);
        else printf("%d\n",vetor[i]);
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int X(int i,int L){
    return i%L;
}
int Y(int i,int L){
    return i/L;
}



int vizinho(int x, int y,int L){// Função que retorna o vizinho y do sitio x
    switch(y){
        case 1: //UP//
            if (x/L!=L-1)return x+L;
            else return -1;
        case 2: //Down//
            if(x>=L) return x-L;
            else return -1;
        case 3: //Right//
            if((x+1)%L!=0) return x+1;
            else return -1;
        case 4: //Left//
            if (x%L!=0) return x-1;
            else return -1;
    }
}