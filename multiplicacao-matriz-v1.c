#include <stdio.h>
#include <mpi.h>


/*
mpicc -g -Wall -o programa multiplicacao-matriz.c
mpiexec -np 2 ./programa


*/


int main(int argc, char **argv)
{
  int p = 2;
  int id;
  int n; 
  double timeInit; 
  double timeEnd; 
  double totalInit; 
  double totalEnd;
  double time;
 

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if(id != 0){
    int X[n/p][n];
    int Y[n/p][n];
    int copia[n/p][n];
    int C[n/p][n];
    int tamanho = n/p;
    int resultado;

    timeInit = MPI_Wtime();
    //Recebe os blocos do processador 0, com Y=B't X=A'
    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Recv(&X[i], n, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Recv(&Y[i], n, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for(int i=0; i<n/p; i++){
      for(int tam=0; tam<tamanho; tam++){
        resultado = 0;
        for(int j=0; j<n; j++){
          resultado +=  X[i][j]*Y[tam][j];
        }
        //printf("resultado [%d][%d] = %d (%d+%d)mod%d*%d/%d+%d\n", i,((id+vezes)%p)*n/p+tam, resultado, id,vezes,p,n,p,tam);
        C[i][id*n/p+tam] = resultado;
      }
    }

    for(int vezes=p-1; vezes>0; vezes--){
      /*for(int i=0; i<n/p; i++){
        for(int j=0; j<n; j++)
          printf("%d(%d) ", X[i][j],id);
        printf("\n");
      }
      printf("\n");
      for(int i=0; i<n/p; i++){
        for(int j=0; j<n; j++)
          printf("%d(%d) ", Y[i][j],id);
        printf("\n");
      }
      printf("\n");
*/
      for(int i=0; i<tamanho; i++)
        for(int j=0; j<n; j++)
          copia[i][j] = Y[i][j];
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Recv(&Y[i], n, MPI_INT, (id-1)%p, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(&copia[i], n, MPI_INT, (id+1)%p, tag, MPI_COMM_WORLD);
      for(int i=0; i<n/p; i++){
        for(int tam=0; tam<tamanho; tam++){
          resultado = 0;
          for(int j=0; j<n; j++){
            resultado +=  X[i][j]*Y[tam][j];
          }
          //printf("resultado [%d][%d] = %d (%d+%d)mod%d*%d/%d+%d\n", i,((id+vezes)%p)*n/p+tam, resultado, id,vezes,p,n,p,tam);
          C[i][((id+vezes)%p)*n/p+tam] = resultado;
        }
      }
    }
  /*  for(int i=0; i<n/p; i++){
      for(int j=0; j<n; j++)
        printf("%d(%d) ", C[i][j],id);
      printf("\n");
    }
    printf("\n");
*/

    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Send(&C[i], n, MPI_INT, 0, tag, MPI_COMM_WORLD);
    timeEnd = MPI_Wtime();
    
    //printf("%lf %lf, %d\n", timeInit, timeEnd, id);
  }
  else{

    FILE *file;
    // open file
    file = fopen(argv[1], "r");

    if(!file) {
     printf("ERROR! Couldn't open file.\n");
     return 0;
    }

    if(!fscanf(file, "%d\n", &n)) return 0;


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
/*
    for(int i=0; i<n; i++){
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

    
  fclose(file);
    /*int A[12][12] = {{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{9,10,11,12,6,1,1,2,3,4,6,1},{13,14,15,16,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{9,10,11,12,6,1,1,2,3,4,6,1},{13,14,15,16,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1}};
    int B[12][12] = {{17,18,19,20,1,3,17,18,19,20,1,3},{21,22,23,24,1,3,17,18,19,20,1,3},{25,26,27,28,1,3,17,18,19,20,1,3},{29,30,31,32,1,3,17,18,19,20,1,3},{1,2,3,4,6,1,17,18,19,20,1,3},{5,6,7,8,6,1,17,18,19,20,1,3},{17,18,19,20,1,3,17,18,19,20,1,3},{21,22,23,24,1,3,17,18,19,20,1,3},{25,26,27,28,1,3,17,18,19,20,1,3},{29,30,31,32,1,3,17,18,19,20,1,3},{1,2,3,4,6,1,17,18,19,20,1,3},{5,6,7,8,6,1,17,18,19,20,1,3}};*/
    int tamanho = n/p;
    int X[n/p][n];
    int Y[n/p][n];
    int copia[n/p][n];
    int C[n/p][n];
    int RESPOSTA[n][n];
    int Bt[n][n];
    int resultado;

    //Transpor a matriz para se adequar a alocacao da memoria de uma matriz e facilitar a transmissão
    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        Bt[j][i] = B[i][j];
      }
    }
 
    timeInit = MPI_Wtime();
    //printf("tamanho=%d p=%d n=%d n/p=%d\n", tamanho,p,n,n/p);
    //Enviar as linhas de A para os processadores
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(A+i+proc*tamanho, n, MPI_INT, proc, tag, MPI_COMM_WORLD);
    //Enviar as colunas de B para os processadores
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(Bt+i+proc*tamanho, n, MPI_INT, proc, tag, MPI_COMM_WORLD);
    //Ler a submatriz local do processador 0
    for(int i=0; i<n/p; i++){
      for(int j=0; j<n; j++){
        X[i][j] = A[i][j];
      }
    }
    for(int i=0; i<n/p; i++){
      for(int j=0; j<n; j++){
        Y[i][j] = Bt[i][j];
      }
    }

    for(int i=0; i<n/p; i++){
      for(int tam=0; tam<tamanho; tam++){
        resultado = 0;
        for(int j=0; j<n; j++){
          resultado +=  X[i][j]*Y[tam][j];
        }
        C[i][id*n/p+tam] = resultado;
      }
    }

    for(int vezes=p-1; vezes>0; vezes--){
      /*for(int i=0; i<n/p; i++){
        for(int j=0; j<n; j++)
          printf("%d(%d) ", X[i][j],id);
        printf("\n");
      }
      printf("\n");
      for(int i=0; i<n/p; i++){
        for(int j=0; j<n; j++)
          printf("%d(%d) ", Y[i][j],id);
        printf("\n");
      }
      printf("\n");
*/
      for(int i=0; i<tamanho; i++)
        for(int j=0; j<n; j++)
          copia[i][j] = Y[i][j];
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(&copia[i], n, MPI_INT, 1, tag, MPI_COMM_WORLD);
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Recv(&Y[i], n, MPI_INT, p-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for(int i=0; i<n/p; i++){
        for(int tam=0; tam<tamanho; tam++){
          resultado = 0;
          for(int j=0; j<n; j++){
            resultado +=  X[i][j]*Y[tam][j];
            //printf("%d*%d+",  X[i][j],Y[tam][j]);
          }
          //printf("resultado [%d][%d] = %d (%d+%d)mod%d*%d/%d+%d\n", i,((id+vezes)%p)*n/p+tam, resultado, id,vezes,p,n,p,tam);
          C[i][((id+vezes)%p)*n/p+tam] = resultado;
        }
      }
    }
/*
    for(int i=0; i<n/p; i++){
      for(int j=0; j<n; j++)
        printf("%d(%d) ", C[i][j],id);
      printf("\n");
    }
    printf("\n");
*/
    for(int i=0; i<tamanho; i++){
      for(int j=0; j<n; j++){
        RESPOSTA[i][j] = C[i][j];
      }
    }

    if(p > 1)
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Recv(&RESPOSTA[proc*n/p+i], n, MPI_INT, proc, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    timeEnd = MPI_Wtime();

    printf("%lf %lf, %d\n", timeInit, timeEnd, id);


    /*for(int i=0; i<n; i++){
      for(int j=0; j<n; j++)  printf("%d\t", RESPOSTA[i][j]);
      printf("\n");
    }*/
  }

  MPI_Reduce(&timeInit, &totalInit, 1, MPI_DOUBLE, MPI_MIN,  0, MPI_COMM_WORLD);
  MPI_Reduce(&timeEnd, &totalEnd, 1, MPI_DOUBLE, MPI_MAX,  0, MPI_COMM_WORLD);

  time = totalEnd - totalInit;
  if (id == 0)
  printf("REDUCE = %lf\n", time);

  MPI_Finalize();

  return 0;
}
