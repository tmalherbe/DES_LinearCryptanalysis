#include<pwd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "des_core.h"
#include "des_io.h"

/**
 * Fonction de chiffrement ecb
 * param1 : Nom du fichier clair
 * param2 : Nom du fichier recevant le chiffré
 * param3 : La clé de chiffrement
 *
 */

void encryption_ecb(char *plain_file, char *cipher_file, BYTE *key){
	ROUND_BLOCK exp_key[16] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	};
 
	//expansion de la clé de chiffrement
	Key_Expansion(key, exp_key);
	burnStack(key, 8);

	//projection mémoire du clair
	MAP map = encode_file(plain_file);
 
	//calcul du padd à ajouter
	unsigned int padd_size = 8 -(map.size%8);
  
  	//bloc temporaire
   	BLOCK block = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

   	//préparation du pointeur recevant le chiffré
   	int T = (map.size)>>3;
   	BYTE *dest_buff = calloc(map.size + padd_size, 1);
   
	int i,j;
	for(i=0;i<T;i++){
	
     	block[0] = map.buffer[8*i    ];
     	block[1] = map.buffer[8*i + 1];
     	block[2] = map.buffer[8*i + 2];
     	block[3] = map.buffer[8*i + 3];
	block[4] = map.buffer[8*i + 4];
	block[5] = map.buffer[8*i + 5];
	block[6] = map.buffer[8*i + 6];
	block[7] = map.buffer[8*i + 7];

    	//chiffrement des du bloc temporaire.
    	des_encryption(exp_key, block);
    
      	dest_buff[8*i    ] = block[0];
      	dest_buff[8*i + 1] = block[1];
      	dest_buff[8*i + 2] = block[2];
      	dest_buff[8*i + 3] = block[3];
	dest_buff[8*i + 4] = block[4];
	dest_buff[8*i + 5] = block[5];
	dest_buff[8*i + 6] = block[6];
	dest_buff[8*i + 7] = block[7];
   	}
	//Dernier bloc
    	BLOCK block1;
	block1[0] = padd_size;
	block1[1] = padd_size;
	block1[2] = padd_size;
	block1[3] = padd_size;
	block1[4] = padd_size;
	block1[5] = padd_size;
	block1[6] = padd_size;
	block1[7] = padd_size;

    	//remplissage du dernier bloc
    	for(j=0;j<8-padd_size;j++){
    	  	block1[j] = map.buffer[8*T + j];
   	 }
    	//chiffrement du dernier bloc
    	des_encryption(exp_key, block1);

    	//copie du dernier bloc dans le chiffré
        dest_buff[8*T    ] = block1[0];
        dest_buff[8*T + 1] = block1[1];
        dest_buff[8*T + 2] = block1[2];
        dest_buff[8*T + 3] = block1[3];
	dest_buff[8*T + 4] = block1[4];
	dest_buff[8*T + 5] = block1[5];
	dest_buff[8*T + 6] = block1[6];
	dest_buff[8*T + 7] = block1[7];

	//destruction de la clé expansée
	for(i=0;i<16;i++){
		burnStack(exp_key[i], 6);
	}

    	//sauvegarde du pointeur contenant le chiffré
    	save_file(dest_buff, cipher_file, map.size + padd_size);   
}

/**
 * Fonction de déchiffrement ecb
 * param1 : Nom du fichier recevant le clair
 * param2 : Nom du fichier chiffré
 * param3 : La clé de chiffrement
 */

void decryption_ecb(char *plain_file, char *cipher_file, BYTE *key){
	ROUND_BLOCK exp_key[16] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	};
	Key_Expansion(key, exp_key);
	burnStack(key, 8);

	//projection mémoire du clair
	MAP map = encode_file(cipher_file);
  
	//bloc temporaire
	BLOCK block = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	int T = (map.size) >> 3;
	BYTE *dest_buff = calloc(map.size, 1);

	int i;
	for(i=0;i<T;i++){

     	block[0] = map.buffer[8*i    ];
     	block[1] = map.buffer[8*i + 1];
     	block[2] = map.buffer[8*i + 2];
     	block[3] = map.buffer[8*i + 3];
	block[4] = map.buffer[8*i + 4];
     	block[5] = map.buffer[8*i + 5];
     	block[6] = map.buffer[8*i + 6];
     	block[7] = map.buffer[8*i + 7];
    
    	des_decryption(exp_key, block);

      	dest_buff[8*i    ] = block[0];
      	dest_buff[8*i + 1] = block[1];
      	dest_buff[8*i + 2] = block[2];
      	dest_buff[8*i + 3] = block[3];
	dest_buff[8*i + 4] = block[4];
      	dest_buff[8*i + 5] = block[5];
      	dest_buff[8*i + 6] = block[6];
      	dest_buff[8*i + 7] = block[7];
   }

   	//destruction de la clé expansée
    	for(i=0;i<16;i++){
      		burnStack(exp_key[i], 6);
    	}
  
   	int padd_size = dest_buff[map.size - 1];
   	save_file(dest_buff, plain_file, map.size - padd_size);
}

/**
 * Fonction de chiffrement cbc
 * param1 : Nom du fichier recevant le clair
 * param2 : Nom du fichier chiffré
 * param3 : La clé de chiffrement
 */

void encryption_cbc(char *plain_file, char *cipher_file, BYTE *key){
	ROUND_BLOCK exp_key[16] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	};

	//expansion de la clé de chiffrement
	Key_Expansion(key, exp_key);
	burnStack(key, 8);

	//projection mémoire du clair
	MAP map = encode_file(plain_file);
  
	//calcul du padd à ajouter
	unsigned char padd_size = 8 -(map.size%8);
  
	//bloc temporaire
	BLOCK block = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	//préparation du pointeur recevant le chiffré
	int T = (map.size)>>3;
	BYTE *dest_buff = calloc(map.size + padd_size + 8, 1);

	//préparation du vecteur d'initialisation.
	//Ce dernier est obtenu en chiffrant un bloc constitué de zéros

	des_encryption(exp_key, block);
	dest_buff[0] = block[0];
	dest_buff[1] = block[1];
	dest_buff[2] = block[2];
	dest_buff[3] = block[3];
	dest_buff[4] = block[4];
	dest_buff[5] = block[5];
	dest_buff[6] = block[6];
	dest_buff[7] = block[7];  

	//copie du clair dans le bloc temporaire
	int i,j;
	for(i=1;i<T+1;i++){

		block[0] = map.buffer[8*(i-1)    ] ^ dest_buff[8*(i-1)    ];
		block[1] = map.buffer[8*(i-1) + 1] ^ dest_buff[8*(i-1) + 1];
		block[2] = map.buffer[8*(i-1) + 2] ^ dest_buff[8*(i-1) + 2];
		block[3] = map.buffer[8*(i-1) + 3] ^ dest_buff[8*(i-1) + 3];
		block[4] = map.buffer[8*(i-1) + 4] ^ dest_buff[8*(i-1) + 4];
		block[5] = map.buffer[8*(i-1) + 5] ^ dest_buff[8*(i-1) + 5];
		block[6] = map.buffer[8*(i-1) + 6] ^ dest_buff[8*(i-1) + 6];
		block[7] = map.buffer[8*(i-1) + 7] ^ dest_buff[8*(i-1) + 7];
 
	//chiffrement aes du bloc temporaire.
	des_encryption(exp_key, block);
    
	//copie du bloc temporaire chiffré dans le chiffré
	dest_buff[8*i    ] = block[0];
      	dest_buff[8*i + 1] = block[1];
      	dest_buff[8*i + 2] = block[2];
      	dest_buff[8*i + 3] = block[3];
	  dest_buff[8*i + 4] = block[4];
      dest_buff[8*i + 5] = block[5];
      dest_buff[8*i + 6] = block[6];
      dest_buff[8*i + 7] = block[7];
   }

   	//Dernier bloc
   	BLOCK block1;
	block1[0] = padd_size;
	block1[1] = padd_size;
	block1[2] = padd_size;
	block1[3] = padd_size;
	block1[4] = padd_size;
	block1[5] = padd_size;
	block1[6] = padd_size;
	block1[7] = padd_size;

	//remplissage du dernier bloc
    for(j=0;j<8-padd_size;j++){
      block1[j] = map.buffer[8*T + j];
    }
	block1[0] ^= dest_buff[8*T    ];
	block1[1] ^= dest_buff[8*T + 1];
	block1[2] ^= dest_buff[8*T + 2];
	block1[3] ^= dest_buff[8*T + 3];
	block1[4] ^= dest_buff[8*T + 4];
	block1[5] ^= dest_buff[8*T + 5];
	block1[6] ^= dest_buff[8*T + 6];
	block1[7] ^= dest_buff[8*T + 7];

    //chiffrement du dernier bloc
	des_encryption(exp_key, block1);

    //copie du dernier bloc dans le chiffré
        dest_buff[8*(T+1)    ] = block1[0];
        dest_buff[8*(T+1) + 1] = block1[1];
        dest_buff[8*(T+1) + 2] = block1[2];
        dest_buff[8*(T+1) + 3] = block1[3];
	dest_buff[8*(T+1) + 4] = block1[4];
        dest_buff[8*(T+1) + 5] = block1[5];
        dest_buff[8*(T+1) + 6] = block1[6];
	dest_buff[8*(T+1) + 7] = block1[7];

    //destruction de la clé expansée
    for(i=0;i<16;i++){
      burnStack(exp_key[i], 6);
    }

    //sauvegarde du pointeur contenant le chiffré
    save_file(dest_buff, cipher_file, map.size + padd_size + 8);
}

/**
 * Fonction de déchiffrement cbc
 * param1 : Nom du fichier recevant le clair
 * param2 : Nom du fichier chiffré
 * param3 : La clé de chiffrement
 */

void decryption_cbc(char * plain_file, char *cipher_file, BYTE *key){
	ROUND_BLOCK exp_key[16] = {
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	};

	//expansion de la clé de chiffrement
	Key_Expansion(key, exp_key);
	burnStack(key, 8);
  
	//projection mémoire du chiffré
	MAP map = encode_file(cipher_file);
	BLOCK block = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	int T = (map.size) >> 3;
	BYTE *dest_buff = calloc(map.size - 8, 1);

	int i;
	for(i=0;i<T-1;i++){

		block[0] = map.buffer[8*(i+1)     ];
		block[1] = map.buffer[8*(i+1)  + 1];
		block[2] = map.buffer[8*(i+1)  + 2];
		block[3] = map.buffer[8*(i+1)  + 3];
		block[4] = map.buffer[8*(i+1)  + 4];
		block[5] = map.buffer[8*(i+1)  + 5];
		block[6] = map.buffer[8*(i+1)  + 6];
		block[7] = map.buffer[8*(i+1)  + 7];

    des_decryption(exp_key, block);

	    dest_buff[8*i    ] = block[0];
		dest_buff[8*i + 1] = block[1];
		dest_buff[8*i + 2] = block[2];
		dest_buff[8*i + 3] = block[3];
		dest_buff[8*i + 4] = block[4];
		dest_buff[8*i + 5] = block[5];
		dest_buff[8*i + 6] = block[6];
		dest_buff[8*i + 7] = block[7];
   }

   //déchiffrement des blocs
   for(i=0;i<T-1;i++){
      dest_buff[8*i    ] ^= map.buffer[8*i    ];
      dest_buff[8*i + 1] ^= map.buffer[8*i + 1];
      dest_buff[8*i + 2] ^= map.buffer[8*i + 2];
      dest_buff[8*i + 3] ^= map.buffer[8*i + 3];
      dest_buff[8*i + 4] ^= map.buffer[8*i + 4];
      dest_buff[8*i + 5] ^= map.buffer[8*i + 5];
      dest_buff[8*i + 6] ^= map.buffer[8*i + 6];
      dest_buff[8*i + 7] ^= map.buffer[8*i + 7];
   }

   //destruction de la clé expansée
    for(i=0;i<16;i++){
      burnStack(exp_key[i], 6);
    }
  
   int padd_size = (int)dest_buff[map.size - 8 - 1];
   save_file(dest_buff, plain_file, map.size - padd_size - 8);
}

int main(int argc, char **argv){
  char prompt[20]= "Entrez la clef DES \n";

  if(argc != 4){
    printf("Usage  : \n>./des mode plaintext ciphertext key \n");
    printf("Le mode est le caractère 'e' pour chiffrer, ou le caractère 'd' pour déchiffrer\n");
    exit(EXIT_SUCCESS);
  }

  if(strcmp(argv[1], "e-ecb")==0){
    char * key = getpass(prompt);
    encryption_ecb(argv[2], argv[3], (BYTE *)key);
    exit(EXIT_SUCCESS);
  }
  if(strcmp(argv[1], "d-ecb")==0){
    char * key = getpass(prompt);
    decryption_ecb(argv[2], argv[3], (BYTE *)key);
    exit(EXIT_SUCCESS);
  }
  if(strcmp(argv[1], "e-cbc")==0){
    char * key = getpass(prompt);
    encryption_cbc(argv[2], argv[3], (BYTE *)key);
    exit(EXIT_SUCCESS);
  }
  if(strcmp(argv[1], "d-cbc")==0){
    char * key = getpass(prompt);
    decryption_cbc(argv[2], argv[3], (BYTE *)key);
    exit(EXIT_SUCCESS);
  }

  if( (strcmp(argv[1], "e-ecb") != 0) && (strcmp(argv[1], "d-ecb") != 0) &&
      (strcmp(argv[1], "e-cbc") != 0) && (strcmp(argv[1], "d-cbc") != 0)
    ){
       printf("Usage  : \n>./des mode plaintext ciphertext key \n");
       exit(EXIT_SUCCESS);
  }
  
  return 0;
}
