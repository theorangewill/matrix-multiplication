compile = mpicc
cflags =  -Wall

sequential: sequential.c
	$(compile) $(cflags) sequential.c -o sequential

version1: parallel-v1.c
	$(compile) $(cflags) parallel-v1.c -o version1

version2: parallel-v2.c
	$(compile) $(cflags) parallel-v2.c -lm -o version2

clean:
	rm sequential version1 version2
