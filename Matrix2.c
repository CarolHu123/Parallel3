#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define thread_count 4

/*void matrix(int a[dim][dim],int n);

void matrix(int a [dim][dim], int n){

    for(int i =0; i<n;i++){
        for(int j =0; j<n;j++){
            a[i][j] = 2;
        }
        
    }
}*/

int main(int argc, char* argv[]){
    
    int dim = atoi(argv[1]);
    int a[dim][dim], b[dim][dim],c[dim][dim];

    for(int i =0; i<dim;i++){
            for(int j =0; j<dim;j++){
                a[i][j] = 2;
                b[i][j] = 3;
                c[i][j]=0;
            }
    }
    for(int i =0; i<dim;i++){
            for(int j =0; j<dim;j++){
                
            }
    }

    double start = omp_get_wtime();
    #pragma omp parallel shared (a, b, c, dim) num_threads(thread_count)
    #pragma omp for schedule(static) collapse(2)
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    double end = omp_get_wtime();
    double elapsed = end - start;
    printf("elapsed time is %f\n",elapsed);

    for(int i =0; i<dim;i++){
            for(int j =0; j<dim;j++){
                printf("%d ",c[i][j]);
            }
            printf("\n");
    }
    return 0;
}
