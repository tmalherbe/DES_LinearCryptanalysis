des_core:
	gcc -g -Wall des_core.c -c

biais:
	gcc -g biais_sboxes.c -o biais_sboxes des_core.o


all:	des_core biais 

clean:
	rm *~ biais_sboxes des_core.o
