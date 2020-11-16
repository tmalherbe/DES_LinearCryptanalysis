#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include"des_core.h"

/**
 *	exploite la meilleure approximation lineaire de la boite-S n°5
 *	pour retrouver un bit de clef.
 *
 */

int feistel_stat(HALF_BLOCK *plaintext, HALF_BLOCK *ciphertext, int N) {
	int T_0 = 0;
	int T_1 = 0;
	BYTE val = 0;
    
	int i;
	for(i=0;i<N;i++){
		val ^= ((plaintext[i][2] & 0x80)>>7)^((ciphertext[i][0] & 0x20)>>5)^((ciphertext[i][0] & 0x01));
		val ^= ((ciphertext[i][1] & 0x04)>>2)^((ciphertext[i][3] & 0x80)>>7);
		if(val == 0){
			T_0++;
		}else{
			T_1++;
		}
	}
	if(T_0 > T_1){
		return 1;
	}else{
		return 0;
	}
}

int main(int argc, char **argv) {

	int i, k;

	HALF_BLOCK plaintext[32];
	HALF_BLOCK ciphertext[32];
	ROUND_BLOCK roundkey;
	srand(getpid());
	for(i=0;i<32;i++){
		for(k=0;k<4;k++){
			plaintext[i][k] = rand() >> 8;
			ciphertext[i][k] = plaintext[i][k];
		}
	}
		
	for(i=0;i<6;i++){
		roundkey[i] = rand() >> 8;
	}
	for(i=0;i<32;i++){
		Feistel_Function(ciphertext[i], roundkey);
	}

	k = feistel_stat(plaintext, ciphertext, 32);
	if( k == ((roundkey[3] & 0x40)>>6) ){
		printf("succes \n");
	}else{
		printf("echec \n");
	}
	return 0;
}
