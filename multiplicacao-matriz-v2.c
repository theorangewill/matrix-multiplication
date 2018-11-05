#include <stdio.h>
#include <mpi.h>
#include <math.h>



/*
522 556 590 624 104 74
890 940 990 1040 120 122
1258 1324 1390 1456 136 170
1626 1708 1790 1872 152 218
522 556 590 624 104 74
890 940 990 1040 120 122

522 556 590 624 104 74
890 940 990 1040 120 122
1258 1324 1390 1456 136 170
1626 1708 1790 1872 152 218
17 21 25 29 1 5
19 23 27 31 3 7

ele altera as duas ultimas linhas da submatriz C, tem os prints entre a parte q altera a matriz

pra compilar:
mpicc -g -Wall -o programa multiplicacao-matriz.c

pra executar: 4 é o numero de processaores, tem q ser potencia de 4. Com 1 processador dá certo (pq nao entra no for vezes=raiz-1 etc...)
mpiexec -np 4 ./programa


*/
int main(void)
{
  int p = 4;
  int id;
  int n = 12;


  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  if(id != 0){
    int raiz = sqrt(p);
    int tamanho = (int) n/raiz;
    int X[tamanho][tamanho];
    int Y[tamanho][tamanho];
    int D[tamanho][tamanho];
    int temp[tamanho];
    int copia[tamanho][tamanho];
    int C[tamanho][n];
    int resultado;
    int linha = id/raiz;
    int coluna = id%raiz;

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
    if(coluna == 0)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Send(&C[i], n, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }
  else{
    //int A[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    //int B[4][4] = {{17,18,19,20},{21,22,23,24},{25,26,27,28},{29,30,31,32}};
    //int A[4][4] = {{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4}};
    //int B[4][4] = {{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4}};
    int A[12][12] = {{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{9,10,11,12,6,1,1,2,3,4,6,1},{13,14,15,16,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1},{9,10,11,12,6,1,1,2,3,4,6,1},{13,14,15,16,6,1,1,2,3,4,6,1},{1,2,3,4,6,1,1,2,3,4,6,1},{5,6,7,8,6,1,1,2,3,4,6,1}};
    int B[12][12] = {{17,18,19,20,1,3,17,18,19,20,1,3},{21,22,23,24,1,3,17,18,19,20,1,3},{25,26,27,28,1,3,17,18,19,20,1,3},{29,30,31,32,1,3,17,18,19,20,1,3},{1,2,3,4,6,1,17,18,19,20,1,3},{5,6,7,8,6,1,17,18,19,20,1,3},{17,18,19,20,1,3,17,18,19,20,1,3},{21,22,23,24,1,3,17,18,19,20,1,3},{25,26,27,28,1,3,17,18,19,20,1,3},{29,30,31,32,1,3,17,18,19,20,1,3},{1,2,3,4,6,1,17,18,19,20,1,3},{5,6,7,8,6,1,17,18,19,20,1,3}};

    int raiz = (int) sqrt(p);
    int tamanho = (int) n/raiz;
    int copia[tamanho][tamanho];
    int X[tamanho][tamanho];
    int Y[tamanho][tamanho];
    int D[tamanho][tamanho];
    int temp[tamanho];
    int C[n/p][n];
    int RESPOSTA[n][n];
    int Bt[n][n];
    int resultado;
    int linha = id/raiz;
    int coluna = id%raiz;
    printf("copia %ld X %ld Y %ld D %ld temp %ld C %ld Bt %ld\n",sizeof(copia),sizeof(X),sizeof(Y),sizeof(D),sizeof(temp),sizeof(C),sizeof(Bt) );

    //Transpor a matriz para se adequar a alocacao da memoria de uma matriz e facilitar a transmissão
    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++){
        Bt[j][i] = B[i][j];
      }
    }
    //printf("tamanho=%d p=%d n=%d n/p=%d\n", tamanho,p,n,n/p);

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
        C[i][coluna*tamanho+j] = D[i][j];
      }
    }

    for(int vezes=raiz-1; vezes>0; vezes--){
      for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
          printf("%d ", C[i][coluna*tamanho+j]);
        }
        printf("\n");
      }
          for(int i=0; i<tamanho; i++){
            for(int j=0; j<tamanho; j++){
              D[i][j] = Y[i][j];
            }
          }

      printf("\n");
      for(int i=0; i<tamanho; i++){
        for(int j=0; j<tamanho; j++){
          printf("%d ", C[i][coluna*tamanho+j]);
        }
        printf("\n");
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

    for(int i=0; i<tamanho; i++){
      for(int j=0; j<n; j++){
        RESPOSTA[i][j] = C[i][j];
      }
    }

    if(p > 1)
    for(int proc=1; proc<raiz; proc++)
      for(int i=0, tag=0; i<tamanho; i++, tag++)
        MPI_Recv(&RESPOSTA[proc*tamanho+i], n, MPI_INT, proc*raiz, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


    for(int i=0; i<n; i++){
      for(int j=0; j<n; j++)  printf("%d\t", RESPOSTA[i][j]);
      printf("\n");
    }
  }
  MPI_Finalize();
  return 0;
}
