#!/bin/bash
make sequencial

./sequencial MAT-100-1.matriz > sequencial-100-1.out
./sequencial MAT-100-2.matriz > sequencial-100-2.out
./sequencial MAT-500-1.matriz > sequencial-500-1.out
./sequencial MAT-500-2.matriz > sequencial-500-2.out
./sequencial MAT-1000-1.matriz > sequencial-1000-1.out
./sequencial MAT-1000-2.matriz > sequencial-1000-2.out

make versao1

mpicc -np 1 ./versao1 MAT-100-1.matriz > versao1-1-100-1.out
mpicc -np 2 ./versao1 MAT-100-1.matriz > versao1-2-100-1.out
mpicc -np 4 ./versao1 MAT-100-1.matriz > versao1-4-100-1.out
mpicc -np 8 ./versao1 MAT-100-1.matriz > versao1-8-100-1.out
mpicc -np 16 ./versao1 MAT-100-1.matriz > versao1-16-100-1.out

mpicc -np 1 ./versao1 MAT-100-2.matriz > versao1-1-100-2.out
mpicc -np 2 ./versao1 MAT-100-2.matriz > versao1-2-100-2.out
mpicc -np 4 ./versao1 MAT-100-2.matriz > versao1-4-100-2.out
mpicc -np 8 ./versao1 MAT-100-2.matriz > versao1-8-100-2.out
mpicc -np 16 ./versao1 MAT-100-2.matriz > versao1-16-100-2.out

mpicc -np 1 ./versao1 MAT-500-1.matriz > versao1-1-500-1.out
mpicc -np 2 ./versao1 MAT-500-1.matriz > versao1-2-500-1.out
mpicc -np 4 ./versao1 MAT-500-1.matriz > versao1-4-500-1.out
mpicc -np 8 ./versao1 MAT-500-1.matriz > versao1-8-500-1.out
mpicc -np 16 ./versao1 MAT-500-1.matriz > versao1-16-500-1.out

mpicc -np 1 ./versao1 MAT-500-2.matriz > versao1-1-500-2.out
mpicc -np 2 ./versao1 MAT-500-2.matriz > versao1-2-500-2.out
mpicc -np 4 ./versao1 MAT-500-2.matriz > versao1-4-500-2.out
mpicc -np 8 ./versao1 MAT-500-2.matriz > versao1-8-500-2.out
mpicc -np 16 ./versao1 MAT-500-2.matriz > versao1-16-500-2.out

mpicc -np 1 ./versao1 MAT-1000-1.matriz > versao1-1-1000-1.out
mpicc -np 2 ./versao1 MAT-1000-1.matriz > versao1-2-1000-1.out
mpicc -np 4 ./versao1 MAT-1000-1.matriz > versao1-4-1000-1.out
mpicc -np 8 ./versao1 MAT-1000-1.matriz > versao1-8-1000-1.out
mpicc -np 16 ./versao1 MAT-1000-1.matriz > versao1-16-1000-1.out

mpicc -np 1 ./versao1 MAT-1000-2.matriz > versao1-1-1000-2.out
mpicc -np 2 ./versao1 MAT-1000-2.matriz > versao1-2-1000-2.out
mpicc -np 4 ./versao1 MAT-1000-2.matriz > versao1-4-1000-2.out
mpicc -np 8 ./versao1 MAT-1000-2.matriz > versao1-8-1000-2.out
mpicc -np 16 ./versao1 MAT-1000-2.matriz > versao1-16-1000-2.out

make versao2

mpicc -np 1 ./versao1 MAT-100-1.matriz > versao1-1-100-1.out
mpicc -np 4 ./versao1 MAT-100-1.matriz > versao1-4-100-1.out
mpicc -np 16 ./versao1 MAT-100-1.matriz > versao1-16-100-1.out

mpicc -np 1 ./versao1 MAT-100-2.matriz > versao1-1-100-2.out
mpicc -np 4 ./versao1 MAT-100-2.matriz > versao1-4-100-2.out
mpicc -np 16 ./versao1 MAT-100-2.matriz > versao1-16-100-2.out

mpicc -np 1 ./versao1 MAT-500-1.matriz > versao1-1-500-1.out
mpicc -np 4 ./versao1 MAT-500-1.matriz > versao1-4-500-1.out
mpicc -np 16 ./versao1 MAT-500-1.matriz > versao1-16-500-1.out

mpicc -np 1 ./versao1 MAT-500-2.matriz > versao1-1-500-2.out
mpicc -np 4 ./versao1 MAT-500-2.matriz > versao1-4-500-2.out
mpicc -np 16 ./versao1 MAT-500-2.matriz > versao1-16-500-2.out

mpicc -np 1 ./versao1 MAT-1000-1.matriz > versao1-1-1000-1.out
mpicc -np 4 ./versao1 MAT-1000-1.matriz > versao1-4-1000-1.out
mpicc -np 16 ./versao1 MAT-1000-1.matriz > versao1-16-1000-1.out

mpicc -np 1 ./versao1 MAT-1000-2.matriz > versao1-1-1000-2.out
mpicc -np 4 ./versao1 MAT-1000-2.matriz > versao1-4-1000-2.out
mpicc -np 16 ./versao1 MAT-1000-2.matriz > versao1-16-1000-2.out
