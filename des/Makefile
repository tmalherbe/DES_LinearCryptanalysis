des_core:
	gcc -g -Wall des_core.c -c

des_io:
	gcc -g -Wall des_io.c -c

des:
	gcc -g -Wall des.c -o des des_core.o des_io.o


all:	des_core des_io des 

clean:
	rm *~ des des_core des_core.o des_io.o
