/**
 *	DES reduit a trois tours, sans permutation initiale.
 *
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"des_core.h"


//Les 8 boites S

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

static BYTE sbox_5[4][16] = {
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

static BYTE shift[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

//Permutation interne à la fonction de Feistel

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

//Les 8 composantes de la boite S

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
 BYTE i = 0;
 BYTE j = 0;

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

//LA boite S

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

//xor de demi-blocs

void Add_Half_Block(HALF_BLOCK left, HALF_BLOCK right){
   int i;
   for(i=0;i<4;i++){
      left[i] ^= right[i];
   }
}

//Permutation de demi-blocs

void swap(BLOCK block) {
	BYTE tmp0, tmp1, tmp2, tmp3;

	tmp0 = block[0];
	tmp1 = block[1];
	tmp2 = block[2];
	tmp3 = block[3];

	block[0] = block[4];
	block[1] = block[5];
	block[2] = block[6];
	block[3] = block[7];
	block[4] = tmp0;
	block[5] = tmp1;
	block[6] = tmp2;
	block[7] = tmp3;
}

//Fonction de Feistel

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

//Tours DES

void DES_Round(BLOCK block, ROUND_BLOCK round_key){
	HALF_BLOCK tmp;
	
	tmp[0] = block[4];
	tmp[1] = block[5];
	tmp[2] = block[6];
	tmp[3] = block[7];

	Feistel_Function(tmp, round_key);
	Add_Half_Block(block, tmp);
	swap(block);

}

//Dernier tour, sans swap

void DES_Final_Round(BLOCK block, ROUND_BLOCK round_key){
	HALF_BLOCK tmp;
	
	tmp[0] = block[4];
	tmp[1] = block[5];
	tmp[2] = block[6];
	tmp[3] = block[7];

	Feistel_Function(tmp, round_key);
	Add_Half_Block(block, tmp);

}

/**
 * Permuted choice 1. Fonction intervenant dans l'expansion.
 * k contient la clef initiale.
 * subk contient 56 "vrais" bits.
 */

void Permuted_Choice_1(BLOCK k) {
	BLOCK tmp = {0, 0, 0, 0, 0, 0, 0, 0};
	tmp[0] = ((k[7]&0x80)     )^((k[6]&0x80) >> 1)^((k[5]&0x80) >> 2)^((k[4]&0x80) >> 3)^((k[3]&0x80) >> 4)^((k[2]&0x80) >> 5)^((k[1]&0x80) >> 6);
    tmp[1] = ((k[0]&0x80)     )^((k[7]&0x40)     )^((k[6]&0x40) >> 1)^((k[5]&0x40) >> 2)^((k[4]&0x40) >> 3)^((k[3]&0x40) >> 4)^((k[2]&0x40) >> 5);
	tmp[2] = ((k[1]&0x40) << 1)^((k[0]&0x40)     )^((k[7]&0x20)     )^((k[6]&0x20) >> 1)^((k[5]&0x20) >> 2)^((k[4]&0x20) >> 3)^((k[3]&0x20) >> 4);
	tmp[3] = ((k[2]&0x20) << 2)^((k[1]&0x20) << 1)^((k[0]&0x20)     )^((k[7]&0x10)     )^((k[6]&0x10) >> 1)^((k[5]&0x10) >> 2)^((k[4]&0x10) >> 3);
	tmp[4] = ((k[7]&0x02) << 6)^((k[6]&0x02) << 5)^((k[5]&0x02) << 4)^((k[4]&0x02) << 3)^((k[3]&0x02) << 2)^((k[2]&0x02) << 1)^((k[1]&0x02)     );
	tmp[5] = ((k[0]&0x02) << 6)^((k[7]&0x04) << 4)^((k[6]&0x04) << 3)^((k[5]&0x04) << 2)^((k[4]&0x04) << 1)^((k[3]&0x04)     )^((k[2]&0x04) >> 1);
	tmp[6] = ((k[1]&0x04) << 5)^((k[0]&0x04) << 4)^((k[7]&0x08) << 2)^((k[6]&0x08) << 1)^((k[5]&0x08)     )^((k[4]&0x08) >> 1)^((k[3]&0x08) >> 2);
	tmp[7] = ((k[2]&0x08) << 4)^((k[1]&0x08) << 3)^((k[0]&0x08) << 2)^((k[3]&0x10)     )^((k[2]&0x10) >> 1)^((k[1]&0x10) >> 2)^((k[0]&0x10) >> 3);
	
	k[0] = tmp[0];
	k[1] = tmp[1];
	k[2] = tmp[2];
	k[3] = tmp[3];
	k[4] = tmp[4];
	k[5] = tmp[5];
	k[6] = tmp[6];
	k[7] = tmp[7];
}

//Op�ration de SHIFT utilis�e dans l'expansion.

void SHIFT(BLOCK sub, int offset){
	HALF_BLOCK tmp1 = {0, 0, 0, 0};
	HALF_BLOCK tmp2 = {0, 0, 0, 0};

	while(offset){
	tmp1[0] = (sub[0] << 1) ^ ((sub[1]&0x80) >> 6);
	tmp1[1] = (sub[1] << 1) ^ ((sub[2]&0x80) >> 6);
	tmp1[2] = (sub[2] << 1) ^ ((sub[3]&0x80) >> 6);
	tmp1[3] = (sub[3] << 1) ^ ((sub[0]&0x80) >> 6);
	tmp2[0] = (sub[4] << 1) ^ ((sub[5]&0x80) >> 6);
	tmp2[1] = (sub[5] << 1) ^ ((sub[6]&0x80) >> 6);
	tmp2[2] = (sub[6] << 1) ^ ((sub[7]&0x80) >> 6);
	tmp2[3] = (sub[7] << 1) ^ ((sub[4]&0x80) >> 6);

	sub[0] = tmp1[0];
	sub[1] = tmp1[1];
	sub[2] = tmp1[2];
	sub[3] = tmp1[3];
	sub[4] = tmp2[0];
	sub[5] = tmp2[1];
	sub[6] = tmp2[2];
	sub[7] = tmp2[3];

	offset--;
	}
}

//Permuted Choice 2. Fonction intervenant dans l'expansion.

void Permuted_Choice_2(BLOCK sub, ROUND_BLOCK round_key) {
	round_key[0] = ((sub[1]&0x02) << 6)^((sub[2]&0x20) << 1)^((sub[1]&0x10) << 1)^((sub[3]&0x20) >> 1)^((sub[0]&0x80) >> 4)^((sub[0]&0x08) >> 1)^((sub[0]&0x20) >> 4)^((sub[3]&0x02) >> 1);
	round_key[1] = ((sub[2]&0x80)     )^((sub[0]&0x04) << 4)^((sub[2]&0x02) << 4)^((sub[1]&0x20) >> 1)^((sub[3]&0x40) >> 3)^((sub[2]&0x08) >> 1)^((sub[1]&0x08) >> 2)^((sub[0]&0x10) >> 4);
	round_key[2] = ((sub[3]&0x08) << 4)^((sub[1]&0x80) >> 1)^((sub[2]&0x40) >> 1)^((sub[0]&0x02) << 3)^((sub[3]&0x04) << 1)^((sub[2]&0x04)     )^((sub[1]&0x04) >> 1)^((sub[0]&0x40) >> 6);
	round_key[3] = ((sub[5]&0x04) << 5)^((sub[7]&0x20) << 1)^((sub[4]&0x20)     )^((sub[5]&0x40) >> 2)^((sub[6]&0x08)     )^((sub[7]&0x04)     )^((sub[4]&0x40) >> 5)^((sub[5]&0x08) >> 3);
	round_key[4] = ((sub[7]&0x40) << 1)^((sub[6]&0x20) << 1)^((sub[4]&0x08) << 2)^((sub[6]&0x04) << 2)^((sub[6]&0x40) >> 3)^((sub[6]&0x02) << 1)^((sub[5]&0x10) >> 3)^((sub[7]&0x02) >> 1);
	round_key[5] = ((sub[4]&0x04) << 5)^((sub[7]&0x10) << 2)^((sub[6]&0x10) << 1)^((sub[5]&0x02) << 3)^((sub[7]&0x80) >> 4)^((sub[5]&0x80) >> 5)^((sub[4]&0x80) >> 6)^((sub[4]&0x10) >> 4);
}

//Expansion de la cl� DES

void Key_Expansion(BLOCK key, ROUND_BLOCK K[16]) {
	int i;
    Permuted_Choice_1(key);
	for(i=0;i<16;i++) {
		SHIFT(key, shift[i]);
		Permuted_Choice_2(key, K[i]);
	}
}

//Chiffrement DES reduit a trois tours

void des_encryption(ROUND_BLOCK exp_key[16], BLOCK plaintext) {
	int i;

	for(i=0; i<2; i++) {
		DES_Round(plaintext, exp_key[i]);
	}
    DES_Final_Round(plaintext, exp_key[2]);
}

//D�chiffrement DES. Identique au chiffrement, modulo l'ordre des cl�s utilis�es.

void des_decryption(ROUND_BLOCK exp_key[16], BLOCK ciphertext) {
	int i;

	for(i=0; i<2; i++) {
		DES_Round(ciphertext, exp_key[2 - i]);
	}
    DES_Final_Round(ciphertext, exp_key[0]);
}


