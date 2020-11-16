des_core:
	gcc -O3 -g -Wall des_core.c -c

linear_cryptanalysis:
	gcc -O3 -g -Wall linear_cryptanalysis.c -o linear_cryptanalysis des_core.o


all:	des_core linear_cryptanalysis

clean:
	rm *~ des_core.o linear_cryptanalysis
