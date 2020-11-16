typedef unsigned char BYTE;
typedef BYTE BLOCK[8];
typedef BYTE HALF_BLOCK[4];
typedef BYTE ROUND_BLOCK[6];

void Key_Expansion(BLOCK key, ROUND_BLOCK K[16]);

void des_encryption(ROUND_BLOCK exp_key[16], BLOCK plaintext);
void des_decryption(ROUND_BLOCK exp_key[16], BLOCK ciphertext);
