compile = mpicc
cflags =  -Wall


sequencial:
	$(compile) $(cflags) sequencial.c -o sequencial

versao1:
	$(compile) $(cflags) multiplicacao-matriz-v1.c -o versao1

versao2:
	$(compile) $(cflags) multiplicacao-matriz-v2.c -lm -o versao2

clean:
	rm sequencial
	rm versao1
	rm versao2
