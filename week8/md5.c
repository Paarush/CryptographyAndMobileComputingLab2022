#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <byteswap.h>

typedef union {
	uint32_t word;
	uint8_t bytes[4];
} converter;

uint32_t K[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

int s[64] = {
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

uint32_t a0 = 0x67452301;   // A
uint32_t b0 = 0xefcdab89;   // B
uint32_t c0 = 0x98badcfe;   // C
uint32_t d0 = 0x10325476;   // D

uint32_t leftrotate (uint32_t x, int n){
	return (x << n) | (x >> (32-n));
}

void md5round(char buffer[64]){
	converter* M = (converter *) buffer;
	uint32_t A = a0;
	uint32_t B = b0;
	uint32_t C = c0;
	uint32_t D = d0;
	for (int i=0; i<64; i++){
		uint32_t F,g;
		if (i<16){
			F = (B & C) | ((~ B) & D);
			g = i;
		} else if (i<32){
			F = (D & B) | ((~ D) & C);
			g = (5*i + 1) % 16;
		} else if (i<48){
			F = B ^ C ^ D;
			g = (3*i + 5) % 16;
		} else if (i<64){
			F = C ^ (B | (~ D));
			g = (7*i) % 16;
		}
		F = F + A + K[i] + M[g].word;  // M[g] must be a 32-bits block
		A = D;
		D = C;
		C = B;
		B = B + leftrotate(F, s[i]);
	}
	a0 = a0 + A;
	b0 = b0 + B;
	c0 = c0 + C;
	d0 = d0 + D;
}

char* md5(char* input){
	uint64_t len = 0;
	char* output = malloc(33*sizeof(char));
	memset(output, 0, 33);
	char buffer[64];
	int i=0;
	for (int i=0; i<strlen(input)/64; i++){
		memset(buffer, 0, 64);
		memcpy(buffer, input+(len/8), 64);
		md5round(buffer);
		len+=512;
	}
	memset(buffer, 0, 64);
	int bal = strlen(input+(len/8));
	memcpy(buffer, input+(len/8), bal);
	len = len+bal*8;
	if (bal<64){
		buffer[bal] = 0x80;
	}
	if (bal<56-1){
		*(uint64_t *)(buffer+56) = len;
		md5round(buffer);
	} else {
		md5round(buffer);
		memset(buffer, 0, 64);
		*(uint64_t *)(buffer+56) = len;
		md5round(buffer);
	}
	sprintf(output, "%08x%08x%08x%08x", __bswap_32(a0), __bswap_32(b0), __bswap_32(c0), __bswap_32(d0));
	return output;
}

int main(int argc, char** argv){
	if (argc!=2){
		printf("This program 1 arguments exactly: not %d\n", argc);
		return -1;
	}
	char* hash = md5(argv[1]);
	printf("%s\n", hash);
	free(hash);
	return 0;
}
