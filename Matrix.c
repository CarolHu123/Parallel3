#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc, char* argv[]){
    
    int dim = atoi(argv[1]);
    int a[dim][dim], b[dim][dim],c[dim][dim];
    int thread_count=atoi(argv[2]);
    for(int i =0; i<dim;i++){
            for(int j =0; j<dim;j++){
                a[i][j] = 2;
                b[i][j] = 3;
                c[i][j]=0;
            }
    }

    double start = omp_get_wtime();
    //#pragma omp  shared (a, b, c, dim) 
    #pragma omp parallel for collapse(3) num_threads(thread_count)
    for (int i = 0; i < dim; i++) 
    {
        for (int j = 0; j < dim; j++) 
        {
            for (int k = 0; k < dim; k++) 
            {
            	printf("i=%d j=%d k=%d I'm thread %d\n", i,j,k, omp_get_thread_num() );  
                c[i][j] += a[i][k] * b[k][j];
            }
        }
       
    }
    double end = omp_get_wtime();
    double elapsed = end - start;
    printf("elapsed time is %f\n",elapsed);

   /* for(int i =0; i<dim;i++){
            for(int j =0; j<dim;j++){
                printf("%d ",c[i][j]);
            }
            printf("\n");
    }*/
    return 0;
}