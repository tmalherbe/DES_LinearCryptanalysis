#include<stdio.h>
#include<stdlib.h>
#include"des_core.h"

typedef unsigned char BYTE;
typedef BYTE HALF_BLOCK[4];
typedef BYTE ROUND_BLOCK[6];

void Permutation(HALF_BLOCK right) {

	HALF_BLOCK tmp = {0, 0, 0, 0};
	
	tmp[0] ^= (right[1] & (0x01)) << 7;
	tmp[0] ^= (right[0] & (0x02)) << 5;
	tmp[0] ^= (right[2] & (0x10)) << 1; 
	tmp[0] ^= (right[2] & (0x08)) << 1;
	tmp[0] ^= (right[3] & (0x08))     ;
	tmp[0] ^= (right[1] & (0x10)) >> 2;
	tmp[0] ^= (right[3] & (0x10)) >> 3;
	tmp[0] ^= (right[2] & (0x80)) >> 7;

	tmp[1] ^= (right[0] & (0x80))     ;
	tmp[1] ^= (right[1] & (0x02)) << 5;
	tmp[1] ^= (right[2] & (0x02)) << 4;
	tmp[1] ^= (right[3] & (0x40)) >> 2;
	tmp[1] ^= (right[0] & (0x08))     ;
	tmp[1] ^= (right[2] & (0x40)) >> 4;
	tmp[1] ^= (right[3] & (0x02))     ;
	tmp[1] ^= (right[1] & (0x40)) >> 6;

	tmp[2] ^= (right[0] & (0x40)) << 1;
	tmp[2] ^= (right[0] & (0x01)) << 6;
	tmp[2] ^= (right[2] & (0x01)) << 5;
	tmp[2] ^= (right[1] & (0x04)) << 2;
	tmp[2] ^= (right[3] & (0x01)) << 3;
	tmp[2] ^= (right[3] & (0x20)) >> 3;
	tmp[2] ^= (right[0] & (0x20)) >> 4;
	tmp[2] ^= (right[1] & (0x80)) >> 7;

	tmp[3] ^= (right[2] & (0x20)) << 2;
	tmp[3] ^= (right[1] & (0x08)) << 3;
	tmp[3] ^= (right[3] & (0x04)) << 3;
	tmp[3] ^= (right[0] & (0x04)) << 2;
	tmp[3] ^= (right[2] & (0x04)) << 1;
	tmp[3] ^= (right[1] & (0x20)) >> 3;
	tmp[3] ^= (right[0] & (0x10)) >> 3;
	tmp[3] ^= (right[3] & (0x80)) >> 7;

	right[0] = tmp[0];
	right[1] = tmp[1];
	right[2] = tmp[2];
	right[3] = tmp[3];

}

void Expansion(HALF_BLOCK right, ROUND_BLOCK expansed) {

   expansed[0] ^= (right[3] & (0x01)) << 7;
   expansed[0] ^= (right[0] & (0x80)) >> 1;
   expansed[0] ^= (right[0] & (0x40)) >> 1;
   expansed[0] ^= (right[0] & (0x20)) >> 1;
   expansed[0] ^= (right[0] & (0x10)) >> 1;
   expansed[0] ^= (right[0] & (0x08)) >> 1;
   expansed[0] ^= (right[0] & (0x10)) >> 3;
   expansed[0] ^= (right[0] & (0x08)) >> 3;

   expansed[1] ^= (right[0] & (0x04)) << 5;
   expansed[1] ^= (right[0] & (0x02)) << 5;
   expansed[1] ^= (right[0] & (0x01)) << 5;
   expansed[1] ^= (right[1] & (0x80)) >> 3;
   expansed[1] ^= (right[0] & (0x01)) << 3;
   expansed[1] ^= (right[1] & (0x80)) >> 5;
   expansed[1] ^= (right[1] & (0x40)) >> 5;
   expansed[1] ^= (right[1] & (0x20)) >> 5;

   expansed[2] ^= (right[1] & (0x10)) << 3;
   expansed[2] ^= (right[1] & (0x08)) << 3;
   expansed[2] ^= (right[1] & (0x10)) << 1;
   expansed[2] ^= (right[1] & (0x08)) << 1;
   expansed[2] ^= (right[1] & (0x04)) << 1;
   expansed[2] ^= (right[1] & (0x02)) << 1;
   expansed[2] ^= (right[1] & (0x01)) << 1;
   expansed[2] ^= (right[2] & (0x80)) >> 7;

   expansed[3] ^= (right[1] & (0x01)) << 7;
   expansed[3] ^= (right[2] & (0x80)) >> 1;
   expansed[3] ^= (right[2] & (0x40)) >> 1;
   expansed[3] ^= (right[2] & (0x20)) >> 1;
   expansed[3] ^= (right[2] & (0x10)) >> 1;
   expansed[3] ^= (right[2] & (0x08)) >> 1;
   expansed[3] ^= (right[2] & (0x10)) >> 3;
   expansed[3] ^= (right[2] & (0x08)) >> 3;

   expansed[4] ^= (right[2] & (0x04)) << 5;
   expansed[4] ^= (right[2] & (0x02)) << 5;
   expansed[4] ^= (right[2] & (0x01)) << 5;
   expansed[4] ^= (right[3] & (0x80)) >> 3;
   expansed[4] ^= (right[2] & (0x01)) << 3;
   expansed[4] ^= (right[3] & (0x80)) >> 5;
   expansed[4] ^= (right[3] & (0x40)) >> 5;
   expansed[4] ^= (right[3] & (0x20)) >> 5;

   expansed[5] ^= (right[3] & (0x10)) << 3;
   expansed[5] ^= (right[3] & (0x08)) << 3;
   expansed[5] ^= (right[3] & (0x10)) << 1;
   expansed[5] ^= (right[3] & (0x08)) << 1;
   expansed[5] ^= (right[3] & (0x04)) << 1;
   expansed[5] ^= (right[3] & (0x02)) << 1;
   expansed[5] ^= (right[3] & (0x01)) << 1;
   expansed[5] ^= (right[0] & (0x80)) >> 7;

}

//xor du bloc droit expansé et de la clé de tour

void AddRoundKey(ROUND_BLOCK round_key, ROUND_BLOCK block){
   int i;
   for(i=0;i<6;i++){
      block[i] ^= round_key[i];
   }
   
}

static BYTE sbox_1[4][16] = {
{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};
static BYTE sbox_2[4][16] = {
{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
};
static BYTE sbox_3[4][16] = {
{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
};
static BYTE sbox_4[4][16] = {
{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
};
static char sbox_5[4][16] = {
{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
};
static BYTE sbox_6[4][16] = {
{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
};
static BYTE sbox_7[4][16] = {
{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
};
static BYTE sbox_8[4][16] = {
{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};

BYTE _sbox_1(BYTE input) {
 BYTE i = 0;
 BYTE j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;
 
 j ^= (input & (0x1E)) >> 1;

 return sbox_1[i][j];
}

BYTE _sbox_2(BYTE input) {
 BYTE i = 0;
 BYTE j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;

 j ^= (input & (0x1E)) >> 1;

 return sbox_2[i][j];
}

BYTE _sbox_3(BYTE input) {
 BYTE i = 0;
 BYTE j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;

 j ^= (input & (0x1E)) >> 1;

 return sbox_3[i][j];
}

BYTE _sbox_4(BYTE input) {
 BYTE i = 0;
 BYTE j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;

 j ^= (input & (0x1E)) >> 1;

 return sbox_4[i][j];
}

BYTE _sbox_5(BYTE input) {
 char i = 0;
 char j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;

 j ^= (input & (0x1E)) >> 1;

 return sbox_5[i][j];
}
BYTE _sbox_6(BYTE input) {
 BYTE i = 0;
 BYTE j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;

 j ^= (input & (0x1E)) >> 1;

 return sbox_6[i][j];
}

BYTE _sbox_7(BYTE input) {
 BYTE i = 0;
 BYTE j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;

 j ^= (input & (0x1E)) >> 1;

 return sbox_7[i][j];
}

BYTE _sbox_8(BYTE input) {
 BYTE i = 0;
 BYTE j = 0;

 i ^= (input & (0x20)) >> 4;
 i ^= (input & (0x01))     ;

 j ^= (input & (0x1E)) >> 1;

 return sbox_8[i][j];
}

void DES_SBOX(ROUND_BLOCK right, HALF_BLOCK output) {
 
 BYTE inp1 = 0;
 BYTE out1 = 0;
 BYTE inp2 = 0;
 BYTE out2 = 0;
 BYTE inp3 = 0;
 BYTE out3 = 0;
 BYTE inp4 = 0;
 BYTE out4 = 0;
 BYTE inp5 = 0;
 BYTE out5 = 0;
 BYTE inp6 = 0;
 BYTE out6 = 0;
 BYTE inp7 = 0;
 BYTE out7 = 0;
 BYTE inp8 = 0;
 BYTE out8 = 0;

 inp1 ^= (right[0] & (0xFC)) >> 2;
 inp2 ^= (right[0] & (0x03)) << 4;
 inp2 ^= (right[1] & (0xF0)) >> 4;
 inp3 ^= (right[1] & (0x0F)) << 2;
 inp3 ^= (right[2] & (0xC0)) >> 6;
 inp4 ^= (right[2] & (0x3F))     ; 
 inp5 ^= (right[3] & (0xFC)) >> 2;
 inp6 ^= (right[3] & (0x03)) << 4;
 inp6 ^= (right[4] & (0xF0)) >> 4;
 inp7 ^= (right[4] & (0x0F)) << 2;
 inp7 ^= (right[5] & (0xC0)) >> 6;
 inp8 ^= (right[5]& (0x3F))      ;
 
 out1 = _sbox_1(inp1); 
 out2 = _sbox_2(inp2); 
 out3 = _sbox_3(inp3); 
 out4 = _sbox_4(inp4); 
 out5 = _sbox_5(inp5); 
 out6 = _sbox_6(inp6); 
 out7 = _sbox_7(inp7); 
 out8 = _sbox_8(inp8); 
 output[0] ^= (out1 & (0x0F)) << 4;
 output[0] ^= (out2 & (0x0F))     ;
 output[1] ^= (out3 & (0x0F)) << 4;
 output[1] ^= (out4 & (0x0F))     ;
 output[2] ^= (out5 & (0x0F)) << 4;
 output[2] ^= (out6 & (0x0F))     ;
 output[3] ^= (out7 & (0x0F)) << 4;
 output[3] ^= (out8 & (0x0F))     ;
 
}

void Feistel_Function(HALF_BLOCK right, ROUND_BLOCK round_key) {
   ROUND_BLOCK expansed = {0, 0, 0, 0, 0, 0};
   Expansion(right, expansed);
   AddRoundKey(round_key, expansed);

   right[0] = 0;
   right[1] = 0;
   right[2] = 0;
   right[3] = 0;

   DES_SBOX(expansed, right);
   Permutation(right);   
}

void search_max_dev() {
	int a,b,i,j;
	char **T;
	char x, tmp1, tmp2, scal1,scal2, a_ex, b_ex;
	char Tex = 0;
	T = malloc(64*sizeof(char *));
	for(a=0;a<64;a++){
		T[a] = calloc(16,1);
	}
	for(a=1;a<64;a++){
		for(b=1;b<16;b++){
			for(x=0;x<64;x++){
				tmp1 = (a & x);
				tmp2 = (b & _sbox_5(x));
				scal1 = ((tmp1 & 0x20)>> 5)^((tmp1 & 0x10)>> 4)^((tmp1 & 0x08)>> 3)^((tmp1 & 0x04)>> 2)^((tmp1 & 0x02)>> 1)^((tmp1 & 0x01)    );
				scal2 = ((tmp2 & 0x08)>> 3)^((tmp2 & 0x04)>> 2)^((tmp2 & 0x02)>> 1)^((tmp2 & 0x01)    );
				if(scal1 == scal2)
					T[a][b] = T[a][b] + 1;
			}
		}
	}
	for(i=1;i<64;i++){
		for(j=1;j<16;j++){
			if((T[i][j] - 32) < 0){
				T[i][j] = 32 - T[i][j];
			} else{
				T[i][j] = T[i][j] - 32;
			}
		}
	}
	for(i=1;i<64;i++){
		for(j=1;j<16;j++){
			if(T[i][j] > Tex){
				Tex = T[i][j];
				a_ex = i;
				b_ex = j;
			}
		}
	}
	printf("\n Deviation maximale : %d \n", Tex);
	printf("\n a : %d \n", a_ex);
	printf("\n b : %d \n", b_ex);
}

int feistel_stat(HALF_BLOCK *plaintext, HALF_BLOCK *ciphertext, int N) {
	int T_0 = 0;
	int T_1 = 0;
	char val;
    
	int i;
	for(i=0;i<N;i++){
		val = ((plaintext[i][3] & 0x80)>>7)^((ciphertext[i][0] & 0x01))^((ciphertext[i][1] & 0x04)>>2)^((ciphertext[i][3] & 0x80)>>7)^((ciphertext[i][0] & 0x20)>>5);
		if(val){
			T_1++;
		}else{
			T_0++;
		}
	}
	printf("\n Nombres de clairs verifiant la relation : %d \n", T_0);
	printf("\n Nombres de clairs ne la verifiant pas : %d  \n", T_1);
	if(T_0 > T_1){
		printf("valeur vraisemblable du bit de clef :  1 \n");
		return 1;
	}else{
		printf("valeur vraisemblable du bit de clef :  0 \n");
		return 0;
	}
}

int main(int argc, char **argv) {
	//search_max_dev();

	int a, i, k;
	int A = 0;
	int seed;
	HALF_BLOCK plaintext[16];
	HALF_BLOCK ciphertext[16];
	ROUND_BLOCK roundkey;

	for(a=1;a<100;a++){
	seed = a;
		for(i=0;i<16;i++){
			for(k=0;k<4;k++){
				seed = (31*seed + 42)%256;
				plaintext[i][k] = seed;
				ciphertext[i][k] = plaintext[i][k];
			}
		}

		for(i=0;i<6;i++){
			roundkey[i] = (12*seed + 17)%256;
		}
		for(i=0;i<16;i++){
			Feistel_Function(ciphertext[i], roundkey);
		}

		k = feistel_stat(plaintext, ciphertext, 16);
		if(k == ((roundkey[4] & 0x40) >> 6)){
			printf("\n succes \n");A++;
		}else{
			printf("\n echec \n");
		}
	}
	printf("\n nombre de succes : %d \n", A);
	return 0;
}
