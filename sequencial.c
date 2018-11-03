#include <stdio.h>

int main()
{
  int n = 6;
  int A[6][6] = {{1,2,3,4,6,1},{5,6,7,8,6,1},{9,10,11,12,6,1},{13,14,15,16,6,1},{1,2,3,4,6,1},{5,6,7,8,6,1}};
  int B[6][6] = {{17,18,19,20,1,3},{21,22,23,24,1,3},{25,26,27,28,1,3},{29,30,31,32,1,3},{1,2,3,4,6,1},{5,6,7,8,6,1}};
  int RESPOSTA[n][n];

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
  return 0;
}
