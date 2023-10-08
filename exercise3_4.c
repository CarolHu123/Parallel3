#include<stdio.h> 
#include<stdlib.h>
#include<omp.h>
#define thread_count 4

#define N 42000 			//我机器内存不够 

float x[N]; // = {0};			//将x数组全置零 
float b[N]; // = {3,1,0};		//设置等式右边值，没设的自动填零 
float A[N][N]; //= {{2,-3,0}, {0,1,1}, {0,0,-5}}; //设置系数数组，行没填的自动填0，列没填的自动填0. 

int  n=10000;
#define ifprint 0 

int main(int argc, char* argv[])  
{  
	for(int i=0;i<n;i++){
   		b[i] = (float)rand();
	   	for(int j=i;j<n;j++)
	   		A[i][j] = (float)rand();
   	}
	double start, end, elapsed;
   //行运算 
   printf("Row-oriented version:\n");
   start = omp_get_wtime();
   for (int row = n-1; row >= 0; row--) { 
	   x[row] = b[row]; 

	   float sum=0.0; 
	   //方法一：使用reduction 
	   #pragma omp parallel for reduction(+:sum) schedule(static) num_threads(thread_count)
	   for (int col = row + 1; col < n; col++) {
	      sum += A[row][col] * x[col]; 
		  ifprint && printf("  col=%d sum=%.2f, I'm Thread %d\n", col,sum, omp_get_thread_num());
	   }
 
/*	   //方法二：使用critical或atomic串行执行 
	   #pragma omp parallel for schedule(static) num_threads(thread_count)   
	   for (int col = row + 1; col < n; col++) {
	   	  long temp = A[row][col] * x[col];
 		  #pragma omp atomic  //随后的一条语句只能串行执行   	 
	      sum += temp; 
		  ifprint && printf("  col=%d sum=%.2f, I'm Thread %d\n", col,sum, omp_get_thread_num());
	   }
*/ 
	   x[row] -= sum;
	   x[row] /= A[row][row]; 
	   ifprint && printf("x(%d)=%.2f sum=%.2f, I'm Thread %d\n", row,x[row], sum, omp_get_thread_num());
   }
   end = omp_get_wtime();
   elapsed = end - start;
   printf("elapsed time is %f\n",elapsed);

	for(int row=0; row<n; row++)
   		ifprint && printf("x(%d)=%.2f ", row, x[row]);
   printf("\n\n");
   //列运算 
   printf("Column-oriented version:\n");
   start = omp_get_wtime();
    #pragma omp parallel for schedule(static) num_threads(thread_count)
	for (int row = 0; row < n; row++)
		x[row] = b[row];
	for (int col = n-1; col >= 0; col--) {
		x[col] /= A[col][col];
		#pragma omp parallel for schedule(static) num_threads(thread_count)
		for (int row = 0; row < col; row++){ 
			x[row] -= A[row][col] * x[col];
			ifprint && printf("  row=%d I'm Thread %d\n", row, omp_get_thread_num());
		} 
	    ifprint &&  printf("x(%d)=%.2f, I'm Thread %d\n", col,x[col], omp_get_thread_num());
	}
   end = omp_get_wtime();
   elapsed = end - start;
   printf("elapsed time is %f\n",elapsed);

   for(int row=0; row<n; row++)
   		ifprint && printf("x(%d)=%.2f ", row, x[row]);
   printf("\n");
	  
   return 0;  
}  
