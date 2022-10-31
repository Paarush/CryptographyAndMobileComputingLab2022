#include <stdio.h>

#include <stdint.h>

union row {
	uint32_t row;
	uint8_t bytes[4];
};

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))
#define ROTL32_x8(x,shift) ((uint32_t)((x) << ((shift)*8)) | ((x) >> (32 - (shift)*8)))
#define ROTR32_x8(x,shift) ((uint32_t)((x) >> ((shift)*8)) | ((x) << (32 - (shift)*8)))

uint8_t gdouble(uint8_t x){
	uint8_t carry = x >> 7;
	return (x << 1)^(carry*0x1B);
}

uint8_t gmult(uint8_t x, uint8_t y){
	uint8_t ret = 0;
	uint8_t temp;
	temp = x;
	if (y%2==1){
		ret ^= temp;
	}
	y = y>>1;
	for (int i=1; i<8; i++){
		temp=gdouble(temp);
		if (y%2==1){
			ret ^= temp;
		}
		y = y>>1;
	}
	return ret;
}

void initialize_aes_sbox(uint8_t sbox[256]) {
	uint8_t p = 1, q = 1;
	
	/* loop invariant: p * q == 1 in the Galois field */
	do {
		/* multiply p by 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		/* divid6670afa3
25ced373
3c5a0f13
74a80a54e q by 3 (equals multiplication by 0xf6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		/* compute the affine transformation */
		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;
}

void subBytes(uint32_t x[4], uint8_t sbox[256]){
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			((union row *)&(x[i]))->bytes[j]=sbox[((union row)(x[i])).bytes[j]];
		}
	}
}

void shiftRows(uint32_t x[4]){
	for (int i=0; i<4; i++){
		x[i]=ROTL32_x8(x[i],i);
	}
}

void unShiftRows(uint32_t x[4]){
	for (int i=0; i<4; i++){
		x[i]=ROTR32_x8(x[i],i);
	}
}

void gunmix_column(uint8_t r[4]) {
	uint8_t t[4];
	uint8_t m[4] = {0x0E, 0x0B, 0x0D, 0x09};
	for (int i=0; i<4; i++){
		t[i] = gmult(r[0],m[(4-i)%4])^gmult(r[1],m[(5-i)%4])^gmult(r[2], m[(6-i)%4])^gmult(r[3], m[(7-i)%4]);
	}
	for (int i=0; i<4; i++){
		r[i]=t[i];
	}
}

void gmix_column(uint8_t r[4]) {
    uint8_t a[4];
    uint8_t b[4];
    uint8_t c;
    uint8_t h;
    /* The array 'a' is simply a copy of the input array 'r'
     * The array 'b' is each element of the array 'a' multiplied by 2
     * in Rijndael's Galois field
     * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */ 
    for (c = 0; c < 4; c++) {
        a[c] = r[c];
        /* h is 0xff if the high bit of r[c] is set, 0 otherwise */
        h = (r[c] >> 7) & 1; /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = r[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= h * 0x1B; /* Rijndael's Galois field */
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

void mixColumn(uint32_t x[4]){
	for (int i=0; i<4; i++){
		uint8_t col[4];
		for (int j=0; j<4; j++){
			col[j] = ((union row)(x[j])).bytes[3-i];
		}
		gmix_column(col);
		for (int j=0; j<4; j++){
			((union row*)&(x[j]))->bytes[3-i] = col[j];
		}
	}
}

void unmixColumn(uint32_t x[4]){
	for (int i=0; i<4; i++){
		uint8_t col[4];
		for (int j=0; j<4; j++){
			col[j] = ((union row)(x[j])).bytes[3-i];
		}
		gunmix_column(col);
		for (int j=0; j<4; j++){
			((union row*)&(x[j]))->bytes[3-i] = col[j];
		}
	}
}

uint8_t rc[] = {0, 0x01 , 0x02 , 0x04 , 0x08 , 0x10 , 0x20 , 0x40 , 0x80 , 0x1B , 0x36};

void keyGen(unsigned int N, uint32_t K[], uint32_t W[], uint8_t sbox[256]){
	int i=0;
	int R=0;
	switch (N){
		case 4:
			R=11;
			break;
		case 6:
			R=13;
			break;
		case 8:
			R=15;
			break;
	}
	for (;i<N; i++){
		W[i]=K[i];
	}
	for (;i<(4*R);i++){
		if (i%N==0){
			uint32_t rot = ROTL32_x8(W[i-1],1);
			for (int j=0; j<4; j++){
				((union row*)&(W[i]))->bytes[3-j]=sbox[((union row)(rot)).bytes[3-j]];
			}
			uint32_t rcon = 0;
			rcon=rc[i/N];
			rcon = rcon << 24;
			W[i]=W[i-N]^W[i]^rcon;
		} else if (N>6 && i%N==4){
			for (int j=0; j<4; j++){
				((union row*)&(W[i]))->bytes[3-j]=sbox[((union row)(W[i-1])).bytes[3-j]];
			}
			W[i]=W[i-N]^W[i];
		} else {
			W[i]=W[i-N]^W[i-1];
		}
	}
}

void addRoundKey(uint32_t x[4], uint32_t R[4]){
	for (int i=0; i<4; i++){
		x[i]=x[i]^R[i];
	}
}

void transpose(uint8_t x[4][4], uint8_t o[4][4]){
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			o[i][3-j]=x[j][3-i];
}

void AESRound(uint32_t x[4], uint32_t R[4], uint8_t sbox[4]){
	subBytes(x, sbox);
	shiftRows(x);
	mixColumn(x);
	addRoundKey(x, R);
}

void unAESRound(uint32_t x[4], uint32_t R[4], uint8_t sbox[4]){
	addRoundKey(x, R);
	unmixColumn(x);
	unShiftRows(x);
	subBytes(x, sbox);
}

void AES(uint32_t x[4], uint32_t K[], unsigned int N){
	uint8_t sbox[256];
	initialize_aes_sbox(sbox);
	int R;
	switch (N){
		case 4:
			R=11;
			break;
		case 6:
			R=13;
			break;
		case 8:
			R=15;
			break;
	}
	uint32_t W[4*R];
	union row temp[4];
	union row itemp[4];
	transpose(&(((union row*)x)->bytes), &(itemp[0].bytes));
	keyGen(N, K, W, sbox);
	transpose(&(((union row*)(W))->bytes), &(temp[0].bytes));
	addRoundKey(&(itemp[0].row), &(temp[0].row));
	for (int i=1; i<R-1; i++){
		transpose(&(((union row*)(W+4*i))->bytes), &(temp[0].bytes));
		AESRound(&(itemp[0].row), &(temp[0].row), sbox);
	}
	transpose(&(((union row*)(W+4*R-4))->bytes), &(temp[0].bytes));
	subBytes(&(itemp[0].row), sbox);
	shiftRows(&(itemp[0].row));
	addRoundKey(&(itemp[0].row), &(temp[0].row));
	transpose(&(itemp[0].bytes), &(((union row*)x)->bytes));
}

void unAES(uint32_t x[4], uint32_t K[], unsigned int N){
	uint8_t sbox[256];
	uint8_t rsbox[256];
	initialize_aes_sbox(sbox);
	for (int i=0; i<256; i++)
		rsbox[sbox[i]]=i;
	int R;
	switch (N){
		case 4:
			R=11;
			break;
		case 6:
			R=13;
			break;
		case 8:
			R=15;
			break;
	}
	uint32_t W[4*R];
	union row temp[4];
	union row itemp[4];
	transpose(&(((union row*)x)->bytes), &(itemp[0].bytes));
	keyGen(N, K, W, sbox);
	transpose(&(((union row*)(W+4*R-4))->bytes), &(temp->bytes));
	addRoundKey(&(itemp[0].row), &(temp[0].row));
	unShiftRows(&(itemp[0].row));
	subBytes(&(itemp[0].row), rsbox);
	for (int i=R-2; i>0; i--){
		transpose(&(((union row*)(W+4*i))->bytes), &(temp[0].bytes));
		unAESRound(&(itemp[0].row), &(temp[0].row), rsbox);
	}
	transpose(&(((union row*)(W))->bytes), &(temp[0].bytes));
	addRoundKey(&(itemp[0].row), &(temp[0].row));
	transpose(&(itemp[0].bytes), &(((union row*)x)->bytes));
}

int main(){
	/* uint32_t key[] = {0x54686174, 0x73206D79, 0x204B756E, 0x67204675}; */
	uint32_t key[] = {0, 0, 0, 0};
	/* uint32_t x[4] = {0x54776F20, 0x4F6E6520, 0x4E696E65, 0x2054776F}; */
	uint32_t x[4] = {0x6A84867C, 0xD77E12AD, 0x07EA1BE8, 0x95C53FA3};
	for (int i=0; i<4; i++){
			printf("%08X", x[i]);
	}
	printf("\n");
	AES(x, key, 4);
	for (int i=0; i<4; i++){
			printf("%08X", x[i]);
	}
	printf("\n");
	unAES(x, key, 4);
	for (int i=0; i<4; i++){
			printf("%08X", x[i]);
	}
	printf("\n");

}
