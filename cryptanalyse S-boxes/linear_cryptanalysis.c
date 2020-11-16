#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include"des_core.h"

int sbox_stat(ROUND_BLOCK *input, HALF_BLOCK *output, int N) {
	int T_0 = 0;
	BYTE val = 0;
    
	int i;
	for(i=0;i<N;i++){
		val = ((input[i][3] & 0x40)>>6)^((output[i][2] & 0x80) >> 7)^((output[i][2] & 0x40) >> 6)^((output[i][2] & 0x20) >> 5)^((output[i][2] & 0x10) >> 4);
		if(val == 0){
			T_0++;
		}
	}
	return T_0;
}

int exp_sbox_perm_stat(HALF_BLOCK *input, HALF_BLOCK *output, int N) {
	int T_0 = 0;
	BYTE val = 0;
    
	int i;
	for(i=0;i<N;i++){
	val = ((input[i][2] & 0x80)>> 7)^((output[i][0] & 0x20)>> 5)^(output[i][0] & 0x01)^((output[i][1] & 0x04)>> 2)^((output[i][3] & 0x80)>> 7);
		if(val == 0){
			T_0++;
		}
	}
	return T_0;
}

int main(int argc, char **argv) {

	int i,k;
	int S = 0;

	ROUND_BLOCK input[32];
	HALF_BLOCK output[32] = {
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
	{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}
	};
	srand(getpid());
	for(i=0;i<32;i++){
		for(k=0;k<6;k++){
			input[i][k] = rand() >> 8;
		}
	}
		
	for(i=0;i<32;i++){
		DES_SBOX(input[i], output[i]);	}

	S = sbox_stat(input, output, 32);
	printf("nombre de couples ok : %d \n", S);

	int T = 0;
	ROUND_BLOCK null_key = {0,0,0,0,0,0};
	HALF_BLOCK input1[32];
	HALF_BLOCK output1[32];

	for(i=0;i<32;i++){
		for(k=0;k<4;k++){
			input1[i][k] = rand() >> 8;
			output1[i][k] = input1[i][k];
		}
	}

	for(i=0;i<32;i++){
		Feistel_Function(output1[i], null_key);
	}

	T = exp_sbox_perm_stat(input1, output1, 32);
		
	printf("nombre de couples youpi : %d \n", T);
	return 0;
}
