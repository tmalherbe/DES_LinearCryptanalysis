typedef unsigned char BYTE;
typedef BYTE BLOCK[8];
typedef BYTE HALF_BLOCK[4];
typedef BYTE ROUND_BLOCK[6];

void AddRoundKey(ROUND_BLOCK round_key, ROUND_BLOCK block);

void Expansion(HALF_BLOCK right, ROUND_BLOCK expansed);

void DES_SBOX(ROUND_BLOCK block, HALF_BLOCK output);

void Permutation(HALF_BLOCK right);

void Add_Half_Block(HALF_BLOCK left, HALF_BLOCK right);

void Feistel_Function(HALF_BLOCK right, ROUND_BLOCK round_key);
