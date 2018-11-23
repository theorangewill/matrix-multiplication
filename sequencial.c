#include <stdio.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  //int n = 12;
  //int A[12][12] = {{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{9,10,11,12,6,1,1,2,3,4,6,1},{13,14,15,16,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{9,10,11,12,6,1,1,2,3,4,6,1},{13,14,15,16,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1}};
  //int B[12][12] = {{17,18,19,20,1,3,17,18,19,20,1,3},{21,22,23,24,1,3,17,18,19,20,1,3},{25,26,27,28,1,3,17,18,19,20,1,3},{29,30,31,32,1,3,17,18,19,20,1,3},{1,2,3,4,6,1,17,18,19,20,1,3},{5,6,7,8,6,1,17,18,19,20,1,3},{17,18,19,20,1,3,17,18,19,20,1,3},{21,22,23,24,1,3,17,18,19,20,1,3},{25,26,27,28,1,3,17,18,19,20,1,3},{29,30,31,32,1,3,17,18,19,20,1,3},{1,2,3,4,6,1,17,18,19,20,1,3},{5,6,7,8,6,1,17,18,19,20,1,3}};
  int n;
  FILE *file;
  double timeInit;
  double timeEnd;
  double time;

  // open file
  file = fopen(argv[1], "r");

  if(!file) {
	 printf("ERROR! Couldn't open file.\n");
	 return 0;
  }
  if(!fscanf(file, "%d\n", &n)) return 0;


  int RESPOSTA[n][n];
  int A[n][n], B[n][n];

  for(int i=0; i<n; i++){
  	for(int j=0; j<n; j++){
        fscanf(file,"%d", &A[i][j]);
    }
      fscanf(file, "\n");
  }


  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
        fscanf(file,"%d", &B[i][j]);
    }
      fscanf(file, "\n");
  }


    /*for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        printf("%d ", A[i][j]);
      }
	printf("\n");
    }

    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        printf("%d ", B[i][j]);
      }
	printf("\n");
    }*/

  //timeInit = MPI_Wtime();
  //clock_t begin = clock();

  timeInit = MPI_Wtime();
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      RESPOSTA[i][j] = 0;
      for(int k=0; k<n; k++) RESPOSTA[i][j] += A[i][k]*B[k][j];
    }
  }

  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++)  printf("%d\t", RESPOSTA[i][j]);
    printf("\n");
  }

  //timeEnd = MPI_Wtime();

  timeEnd = MPI_Wtime();
  //clock_t end = clock();
  //time = end - begin;

  time = timeEnd - timeInit;
  //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  //printf("%lf %lf %lf\n", time, timeInit, timeEnd);
  printf("%lf\n", time);
    fclose(file);
  return 0;
}
