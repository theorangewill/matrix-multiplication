## Matrix multiplication
This is a matrix multiplication parallelized with MPI.

```
generator.c: a program that generates two matrices with the same dimension (variable 'n')
sequential.c: sequential multiplication
parallel-v1.c: the parallelization in which each thread has a row from the first matrix and a column of the other
parallel-v2.c: the parallelization in which each thread has a submatrix with the dimension of n/sqrt(num_threads)
```