#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "des_core.h"

/**
 *	exploite l'approximation linéaire du DES a 7 tours
 *	pour retrouver 6 bits de la dernière sous-clé
 *
 */


BYTE des_8_linear_cryptanalysis(BLOCK *p, BLOCK *c, int N){
 int a,i;
 int T[64];
 for(a=0;a<64;a++)
	T[a] = 0;
 
 BYTE k,x,y;
 BYTE val;
 int T_max = 0;
 BYTE k_max;

 for(k=0;k<64;k++){

  for(i=0;i<N;i++){
   val = ((p[i][0] &0x01)   )^((p[i][1] &0x04)>>2)^((p[i][3] &0x80)>>7)^((p[i][5] &0x01)   )^((p[i][6] &0x10)>>4)^
	 ((c[i][2] &0x80)>>7)^((c[i][4] &0x20)>>5)^((c[i][4] &0x01)   )^((c[i][5] &0x04)>>2)^((c[i][7] &0x80)>>7);

   x = ((c[i][7] &0x01)<<5)^((c[i][4] &0x80)>>3)^((c[i][4] &0x40)>>3)^((c[i][4] &0x20)>>3)^((c[i][4] &0x10)>>3)^((c[i][4] &0x08)>>3)^k;
   y = ((_sbox_1(x) &0x04)>>2);
   val ^= y;

   if(val == 0)
    T[k]++;
  }   

 }
 for(k=0;k<64;k++){
  T[k] = T[k] - N;
  if(T[k] < 0)
  T[k] = -T[k];
 }

 for(k=0;k<64;k++){
   if(T[k] > T_max){
    T_max = T[k];
    k_max = k;
   }
 }

 return k_max;
}

int main(int argc, char **argv) {

	int i, k;
	int tmp;
	BLOCK *plaintext = malloc(3000000*sizeof(BLOCK));
	BLOCK *ciphertext = malloc(3000000*sizeof(BLOCK));
	
	BLOCK key;
	BYTE guessed_key;

	ROUND_BLOCK exp_key[16] = {
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}
	};

	srand(getpid());
	
	for(i=0;i<3000000;i++){
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
	for(i=0;i<3000000;i++){
		des_encryption(exp_key, ciphertext[i]);
	}

	guessed_key = des_8_linear_cryptanalysis(plaintext, ciphertext, 3000000);
	if (guessed_key ==  ((exp_key[7][0] &0xFC) >> 2)  ){
		printf("succes \n");
	}else{
		printf("echec \n");
	}
	return 0;
}
