typedef unsigned char BYTE;
typedef BYTE BLOCK[8];
typedef BYTE HALF_BLOCK[4];
typedef BYTE ROUND_BLOCK[6];

void Initial_Permutation(BLOCK block);

void Inverse_Initial_Permutation(BLOCK block);

void AddRoundKey(ROUND_BLOCK round_key, ROUND_BLOCK block);

void Expansion(HALF_BLOCK right, ROUND_BLOCK expansed);

void DES_SBOX(ROUND_BLOCK block, HALF_BLOCK output);

void Permutation(HALF_BLOCK right);

void Add_Half_Block(HALF_BLOCK left, HALF_BLOCK right);

void DES_Round(BLOCK block, ROUND_BLOCK round_key);

void Key_Expansion(BYTE key[8], ROUND_BLOCK K[16]);

void swap(BLOCK block);
void Feistel_Function(HALF_BLOCK right, ROUND_BLOCK round_key);
void DES_Round(BLOCK block, ROUND_BLOCK round_key);

void des_encryption(ROUND_BLOCK exp_key[16], BLOCK plaintext);
void des_decryption(ROUND_BLOCK exp_key[16], BLOCK ciphertext);
