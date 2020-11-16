#include<stdio.h>
#include<stdlib.h>
#include"des_core.h"

BYTE _sbox(BYTE input, int index){
	switch(index){
		case 1:
			return _sbox_1(input);
			break;
		case 2:
			return _sbox_2(input);
			break;
		case 3:
			return _sbox_3(input);
			break;
		case 4:
			return _sbox_4(input);
			break;
		case 5:
			return _sbox_5(input);
			break;
		case 6:
			return _sbox_6(input);
			break;
		case 7:
			return _sbox_7(input);
			break;
		case 8:
			return _sbox_8(input);
			break;
		default:
			return -1;
			break;
	}
}

void search_max_dev(int index) {
	char a,b,i,j;
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
				tmp2 = (b & _sbox(x,index));
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

int main(int argc, char **argv) {
	search_max_dev(1);
	search_max_dev(2);
	search_max_dev(3);
	search_max_dev(4);
	search_max_dev(5);
	search_max_dev(6);
	search_max_dev(7);
	search_max_dev(8);

	return 0;
}
