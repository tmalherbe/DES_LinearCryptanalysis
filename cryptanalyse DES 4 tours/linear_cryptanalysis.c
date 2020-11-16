#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "des_core.h"

/**
 *	exploite la meilleure approximation lineaire du DES a quatre tours
 *	pour retrouver un bit d'information sur la clef
 *
 */

BYTE des_4_linear_cryptanalysis(BLOCK *plaintext, BLOCK *ciphertext, int N){
	int T_0 = 0;
	int T_1 = 0;
	char val = 0;
	int i;
	for(i=0;i<N;i++){

		val =   ((plaintext[i][0] &0x20)>> 5)^((plaintext[i][0] &0x01)    )^
			((plaintext[i][1] &0x04)>> 2)^((plaintext[i][3] &0x80)>> 7)^
			((plaintext[i][6] &0x80)>> 7)^((ciphertext[i][2] &0x80)>> 7)^
			((ciphertext[i][4] &0x80)>> 7)^((ciphertext[i][4] &0x40)>> 6)^
			((ciphertext[i][4] &0x20)>> 5)^((ciphertext[i][4] &0x10)>> 4)^
			((ciphertext[i][4] &0x08)>> 3)^((ciphertext[i][4] &0x01)    )^
			((ciphertext[i][5] &0x04)>> 2)^((ciphertext[i][7] &0x80)>> 7);

	if(val == 0){
			T_0++;
		}else{
			T_1++;
		}
	}

	if(T_0 > T_1){
		return (BYTE)1;
	}else{
		return (BYTE)0;
	}
}

int main(int argc, char **argv) {

	int i,k;
	int tmp;
	BLOCK plaintext[300];
	BLOCK ciphertext[300];
	BLOCK key;
	BYTE res;

	ROUND_BLOCK exp_key[16] = {
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}
	};

	srand(getpid());
	
	for(i=0;i<300;i++){
		for(k=0;k<8;k++){
			tmp = rand() >> 8;
			plaintext[i][k] = (BYTE)tmp;
			ciphertext[i][k] = plaintext[i][k];
		}
	}

	for(i=0;i<8;i++){
		tmp = rand() >> 8;
		key[i] = (BYTE)tmp;
	}
	Key_Expansion(key, exp_key);
	for(i=0;i<300;i++){
		des_encryption(exp_key, ciphertext[i]);
	}
	res = des_4_linear_cryptanalysis(plaintext, ciphertext, 300);
	if (res == (
		  ((exp_key[0][3] &0x40)>> 6)^((exp_key[2][3] &0x40)>> 6)^
		  ((exp_key[3][0] &0x40)>> 6)^((exp_key[3][0] &0x20)>> 5)^
		  ((exp_key[3][0] &0x08)>> 3)^((exp_key[3][0] &0x04)>> 2)  
		 ) 
	   ){
		printf("succes \n");
	}else{
		printf("echec \n");
	}
	return 0;
}
