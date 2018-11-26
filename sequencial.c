#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  int n;
  FILE *file;
  double timeInit;
  double timeEnd;
  double time;
  int **RESPOSTA;
  int **A, **B;

  // Abre arquivo
  file = fopen(argv[1], "r");
  if(!file) {
	 printf("ERROR! Couldn't open file.\n");
	 return 0;
  }

  //Le o tamanho das matrizes
  if(!fscanf(file, "%d\n", &n)) return 0;

  //Aloca memória para as matrizes
  RESPOSTA = malloc(sizeof(int)*n);
  A = malloc(sizeof(int)*n);
  B = malloc(sizeof(int)*n);
  for(int i=0 ; i<n ; i++){
    RESPOSTA[i] = malloc(sizeof(int)*n);
    A[i] = malloc(sizeof(int)*n);
    B[i] = malloc(sizeof(int)*n);
  }

  //Leitura das matrizes
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

  fclose(file);


  //Inicia o algoritmo sequencial
  timeInit = MPI_Wtime();
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      RESPOSTA[i][j] = 0;
      for(int k=0; k<n; k++) RESPOSTA[i][j] += A[i][k]*B[k][j];
    }
  }
  timeEnd = MPI_Wtime();

  //Imprime na tela a resposta
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++)  printf("%d\t", RESPOSTA[i][j]);
    printf("\n");
  }

  //Calcula o tempo de execução
  time = timeEnd - timeInit;
  printf("%lf\n", time);
  //Libera a memória alocada
  for(int i=0 ; i<n ; i++){
    free(RESPOSTA[i]);
    free(A[i]);
    free(B[i]);
  }
  free(RESPOSTA);
  free(A);
  free(B);
  
  return 0;
}
