#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>

/*
mpicc -g -Wall -o programa multiplicacao-matriz-v2.c -lm
mpiexec -np 4 ./programa
*/

int main(int argc, char **argv)
{
  int p = 4;
  int id;
  int n = 12;

  double timeInit; 
  double timeEnd; 
  double totalInit; 
  double totalEnd;
  double time;


  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  if(id != 0){
    int raiz = (int) sqrt(p);
    int tamanho;
    int **X, **Y, **D, **C;
    int temp;
    int **copia;
    int resultado;
    int linha = id/raiz;
    int coluna = id%raiz;

    //Recebe o valor de N (tamanho das matrizes)
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    tamanho = (int) n/raiz;

    //Aloca a memória necessária para as matrizes
    X = malloc(sizeof(int)*tamanho);
    Y = malloc(sizeof(int)*tamanho);
    copia = malloc(sizeof(int)*tamanho);
    C = malloc(sizeof(int)*tamanho);
    D = malloc(sizeof(int)*tamanho);
    temp = malloc(sizeof(int)*tamanho);
    for(int i=0 ; i<tamanho; i++){
      X[i] = malloc(sizeof(int)*tamanho);
      Y[i] = malloc(sizeof(int)*tamanho);
      copia[i] = malloc(sizeof(int)*tamanho);
      C[i] = malloc(sizeof(int)*n);
      D[i] = malloc(sizeof(int)*tamanho);
    }

    //Recebe os blocos do processador 0, com Y=B't X=A'
    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Recv(&X[i], tamanho, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for(int i=0, tag=0; i<tamanho; i++, tag++)
      MPI_Recv(&Y[i], tamanho, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for(int i=0; i<tamanho; i++){
      for(int tam=0; tam<tamanho; tam++){
        resultado = 0;
        for(int j=0; j<tamanho; j++){
          resultado +=  X[i][j]*Y[tam][j];
        }
        D[i][tam] = resultado;
      }
    }


    //Inicia o algoritmo
    timeInit = MPI_Wtime();
    if(coluna == 0){
      for(int proc=1; proc<raiz; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++){
        MPI_Recv(&temp, tamanho, MPI_INT, proc+linha*raiz, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int j=0; j<tamanho; j++){
          D[i][j] += temp[j];
        }
      }
      for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
          C[i][linha*tamanho+j] = D[i][j];
        }
      }
    }
    else{
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(D[i], tamanho, MPI_INT, linha*raiz, tag, MPI_COMM_WORLD);
    }

    for(int vezes=raiz-1; vezes>0; vezes--){
      for(int i=0; i<tamanho; i++)
        for(int j=0; j<tamanho; j++)
          copia[i][j] = Y[i][j];
      if(linha == 0){
        for(int i=0, tag=0; i<tamanho; i++, tag++)
          MPI_Send(&copia[i], tamanho, MPI_INT, ((unsigned int) (linha+1)%raiz)*raiz+coluna, tag, MPI_COMM_WORLD);
        for(int i=0, tag=0; i<tamanho; i++, tag++)
          MPI_Recv(&Y[i], tamanho, MPI_INT, ((unsigned int) (linha-1)%raiz)*raiz+coluna, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
      else{
        for(int i=0, tag=0; i<tamanho; i++, tag++)
          MPI_Recv(&Y[i], tamanho, MPI_INT, ((unsigned int) (linha-1)%raiz)*raiz+coluna, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i=0, tag=0; i<tamanho; i++, tag++)
          MPI_Send(&copia[i], tamanho, MPI_INT, ((unsigned int) (linha+1)%raiz)*raiz+coluna, tag, MPI_COMM_WORLD);
      }

      for(int i=0; i<tamanho; i++){
        for(int tam=0; tam<tamanho; tam++){
          resultado = 0;
          for(int j=0; j<tamanho; j++){
            resultado +=  X[i][j]*Y[tam][j];
          }
          D[i][tam] = resultado;
        }
      }
      if(coluna == 0){
        for(int proc=1; proc<raiz; proc++)
        for(int i=0, tag=0; i<tamanho; i++, tag++){
          MPI_Recv(&temp, tamanho, MPI_INT, proc+linha*raiz, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          for(int j=0; j<tamanho; j++){
            D[i][j] += temp[j];
          }
        }
      }
      else{
        for(int i=0, tag=0; i<tamanho; i++, tag++)
          MPI_Send(D[i], tamanho, MPI_INT, linha*raiz, tag, MPI_COMM_WORLD);
      }


      for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
          C[i][((linha+vezes)%raiz)*tamanho+j] = D[i][j];
        }
      }
    }
    timeEnd = MPI_Wtime();

    //Envia para o processador 0 a resposta local
    if(coluna == 0)
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
    int raiz = (int) sqrt(p);
    int tamanho;
    int **A, **B, **Bt;
    int **X, **Y, **D, **C;
    int *temp;
    int **RESPOSTA;
    int resultado;
    int linha = id/raiz;
    int coluna = id%raiz;

    FILE *file;
    // open file
    file = fopen(argv[1], "r");

    if(!file) {
     printf("ERROR! Couldn't open file.\n");
     return 0;
    }

    //Le o tamanho das amtrizes
    if(!fscanf(file, "%d\n", &n)) return 0;
    tamanho = (int) n/raiz;

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
    C = malloc(sizeof(int)*tamanho);
    for(int i=0 ; i<tamanho; i++){
      X[i] = malloc(sizeof(int)*tamanho);
      Y[i] = malloc(sizeof(int)*tamanho);
      D[i] = malloc(sizeof(int)*tamanho);
      C[i] = malloc(sizeof(int)*n);
    }

    //Le as matrizes
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

    //Transpor a matriz para se adequar a alocacao da memoria de uma matriz e facilitar a transmissão
    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        Bt[j][i] = B[i][j];
      }
    }



    //Enviar o valor de N para os processadores
    for(int proc=1; proc<p; proc++)
      MPI_Send(&n, 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
    //Enviar as linhas de A para os processadores
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(A[((int) (proc/raiz))*tamanho+i]+((int) (proc%raiz))*tamanho, tamanho, MPI_INT, proc, tag, MPI_COMM_WORLD);
    //Enviar as colunas de B para os processadores
    for(int proc=1; proc<p; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(Bt[((int) (proc/raiz))*tamanho+i]+((int) (proc%raiz))*tamanho, tamanho, MPI_INT, proc, tag, MPI_COMM_WORLD);

    //Ler a submatriz local do processador 0
    for(int i=0; i<tamanho; i++){
      for(int j=0; j<tamanho; j++){
        X[i][j] = A[i][j];
      }
    }
    for(int i=0; i<tamanho; i++){
      for(int j=0; j<tamanho; j++){
        Y[i][j] = Bt[i][j];
      }
    }

    //Iniciar o algoritmo
    timeInit = MPI_Wtime();
    for(int i=0; i<tamanho; i++){
      for(int tam=0; tam<tamanho; tam++){
        resultado = 0;
        for(int j=0; j<tamanho; j++){
          resultado +=  X[i][j]*Y[tam][j];
        }
        D[i][tam] = resultado;
      }
    }

    for(int proc=1; proc<raiz; proc++){
      for(int i=0, tag=0; i<tamanho; i++, tag++){
        MPI_Recv(&temp, tamanho, MPI_INT, proc+linha*raiz, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int j=0; j<tamanho; j++){
          D[i][j] += temp[j];
        }
      }
    }

    for(int i=0; i<tamanho; i++){
      for(int j=0; j<tamanho; j++){
        C[i][coluna*tamanho+j] = D[i][j];
      }
    }

    for(int vezes=raiz-1; vezes>0; vezes--){
      for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
          D[i][j] = Y[i][j];
        }
      }

      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(&D[i], tamanho, MPI_INT, ((unsigned int) (linha+1)%raiz)*raiz+coluna, tag, MPI_COMM_WORLD);
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Recv(&Y[i], tamanho, MPI_INT, ((unsigned int) (linha-1)%raiz)*raiz+coluna, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      for(int i=0; i<tamanho; i++){
        for(int tam=0; tam<tamanho; tam++){
          resultado = 0;
          for(int j=0; j<tamanho; j++){
            resultado +=  X[i][j]*Y[tam][j];
          }
          D[i][tam] = resultado;
        }
      }
      for(int proc=1; proc<raiz; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++){
        MPI_Recv(&temp, tamanho, MPI_INT, proc+linha*raiz, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int j=0; j<tamanho; j++){
          D[i][j] += temp[j];
        }
      }

      for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
          C[i][((linha+vezes)%raiz)*tamanho+j] = D[i][j];
        }
      }
    }
    timeEnd = MPI_Wtime();

    //Recebe as respostas locais
    for(int i=0; i<tamanho; i++){
      for(int j=0; j<n; j++){
        RESPOSTA[i][j] = C[i][j];
      }
    }

    for(int proc=1; proc<raiz; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Recv(&RESPOSTA[proc*tamanho+i], n, MPI_INT, proc*raiz, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

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
