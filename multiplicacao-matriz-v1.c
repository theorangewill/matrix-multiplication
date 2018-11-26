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
 
  //Inicia o MPI
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);


  if(id != 0){
    int tamanho;
    int **X;
    int **Y;
    int **copia;
    int **C;
    int resultado;
    //Recebe o valor de N (tamanho das matrizes)
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    tamanho = (int) n/p;
    //Aloca a memória necessária para as matrizes
    X = malloc(sizeof(int)*tamanho);
    Y = malloc(sizeof(int)*tamanho);
    copia = malloc(sizeof(int)*tamanho);
    C = malloc(sizeof(int)*tamanho);
    for(int i=0 ; i<tamanho; i++){
      X[i] = malloc(sizeof(int)*n);
      Y[i] = malloc(sizeof(int)*n);
      copia[i] = malloc(sizeof(int)*n);
      C[i] = malloc(sizeof(int)*n);
    }

    //Recebe os blocos do processador 0, com Y=B't X=A'
    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Recv(&X[i], n, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Recv(&Y[i], n, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    //Inicia o algoritmo
    timeInit = MPI_Wtime();
    for(int i=0; i<tamanho; i++){
      for(int tam=0; tam<tamanho; tam++){
        resultado = 0;
        for(int j=0; j<n; j++){
          resultado +=  X[i][j]*Y[tam][j];
        }
        C[i][id*n/p+tam] = resultado;
      }
    }


    for(int vezes=p-1; vezes>0; vezes--){
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
          C[i][((id+vezes)%p)*tamanho+tam] = resultado;
        }
      }
    }
    timeEnd = MPI_Wtime();

    //Envia para a resposta local para o processador 0
    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Send(&C[i], n, MPI_INT, 0, tag, MPI_COMM_WORLD);
    
    
    //Libera a memória alocada
    for(int i=0 ; i<tamanho; i++){
      free(X[i]);
      free(Y[i]);
      free(copia[i]);
      free(C[i]);
    }
    free(X);
    free(Y);
    free(copia);
    free(C);

  }
  else{
    int **A, **B, **Bt;
    FILE *file;
    int tamanho;
    int **X, **Y, **copia, **C;
    int **RESPOSTA;
    int resultado;

    //Abre o arquivo
    file = fopen(argv[1], "r");
    if(!file) {
     printf("ERROR! Couldn't open file.\n");
     return 0;
    }

    //Le o tamanho das matrizes
    if(!fscanf(file, "%d\n", &n)) return 0;
    tamanho = (int) n/p;

    //Aloca a memória necessária para as matrizes
    RESPOSTA = malloc(sizeof(int)*n);
    A = malloc(sizeof(int)*n);
    B = malloc(sizeof(int)*n);
    Bt = malloc(sizeof(int)*n);
    for(int i=0 ; i<n ; i++){
      RESPOSTA[i] = malloc(sizeof(int)*n);
      A[i] = malloc(sizeof(int)*n);
      B[i] = malloc(sizeof(int)*n);
      Bt[i] = malloc(sizeof(int)*n);
    }
    X = malloc(sizeof(int)*tamanho);
    Y = malloc(sizeof(int)*tamanho);
    copia = malloc(sizeof(int)*tamanho);
    C = malloc(sizeof(int)*tamanho);
    for(int i=0 ; i<tamanho; i++){
      X[i] = malloc(sizeof(int)*n);
      Y[i] = malloc(sizeof(int)*n);
      copia[i] = malloc(sizeof(int)*n);
      C[i] = malloc(sizeof(int)*n);
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

    //Transpor a matriz para se adequar a alocacao da memoria de uma matriz e facilitar a transmissão
    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        Bt[j][i] = B[i][j];
      }
    }
 
    //Envia o valor de N para os processadores
    for(int proc=1; proc<p; proc++)
      MPI_Send(&n, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
    //Enviar as linhas de A para os processadores
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(A+i+proc*tamanho, n, MPI_INT, proc, tag, MPI_COMM_WORLD);
    //Enviar as colunas de B para os processadores
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(Bt+i+proc*tamanho, n, MPI_INT, proc, tag, MPI_COMM_WORLD);

    //Ler a submatriz local do processador 0
    for(int i=0; i<tamanho; i++){
      for(int j=0; j<n; j++){
        X[i][j] = A[i][j];
      }
    }
    for(int i=0; i<tamanho; i++){
      for(int j=0; j<n; j++){
        Y[i][j] = Bt[i][j];
      }
    }

    //Inicia o algoritmo
    timeInit = MPI_Wtime();
    for(int i=0; i<tamanho; i++){
      for(int tam=0; tam<tamanho; tam++){
        resultado = 0;
        for(int j=0; j<n; j++){
          resultado +=  X[i][j]*Y[tam][j];
        }
        C[i][id*n/p+tam] = resultado;
      }
    }

    for(int vezes=p-1; vezes>0; vezes--){
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
          }
          C[i][((id+vezes)%p)*tamanho+tam] = resultado;
        }
      }
    }

    timeEnd = MPI_Wtime();

    //Recebe as respostas locais dos processadores
    for(int i=0; i<tamanho; i++){
      for(int j=0; j<n; j++){
        RESPOSTA[i][j] = C[i][j];
      }
    }

    if(p > 1)
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Recv(&RESPOSTA[proc*n/p+i], n, MPI_INT, proc, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    //Imprime na tela a resposta
    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++)  printf("%d\t", RESPOSTA[i][j]);
      printf("\n");
    }

    //Libera a memória alocada
    for(int i=0 ; i<tamanho; i++){
      free(X[i]);
      free(Y[i]);
      free(copia[i]);
      free(C[i]);
    }
    free(X);
    free(Y);
    free(copia);
    free(C);
    for(int i=0 ; i<n ; i++){
      free(RESPOSTA[i]);
      free(A[i]);
      free(B[i]);
    }
    free(RESPOSTA);
    free(A);
    free(B);
  }

  //Calcula o tempo do algoritmo
  MPI_Reduce(&timeInit, &totalInit, 1, MPI_DOUBLE, MPI_MIN,  0, MPI_COMM_WORLD);
  MPI_Reduce(&timeEnd, &totalEnd, 1, MPI_DOUBLE, MPI_MAX,  0, MPI_COMM_WORLD);
  if (id == 0){
    time = totalEnd - totalInit;
    printf("%lf\n", time);
  }

  MPI_Finalize();

  return 0;
}
