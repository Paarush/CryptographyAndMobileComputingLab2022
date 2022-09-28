#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ip[] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7,
};

int fp[] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25,
};

int e[] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1,
};

int p[] = {
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25,
};

int pc1l[] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
};

int pc1r[] = {
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4,
};

int pc2[] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32,
};

unsigned long sub(unsigned long n, int map[4][16]){
	int o = 0b100001;
	int i = 0b011110;
	o = n & o;
	i = n & i;
	int x=0,y=0;
	switch (o){
		case 0b000000:
			y=0;
			break;
		case 0b000001:
			y=1;
			break;
		case 0b100000:
			y=2;
			break;
		case 0b100001:
			y=3;
			break;
	}
	x = i >> 1;
	return map[y][x];
}

int s[8][4][16] = {
	{
		{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, },
		{ 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, },
		{ 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, },
		{ 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13, },
	},
	{
		{ 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, },
		{ 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, },
		{ 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, },
		{ 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9, },
	},
	{
		{ 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, },
		{ 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, },
		{ 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, },
		{ 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12, },
	},
	{
		{ 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, },
		{ 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, },
		{ 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, },
		{ 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14, },
	},
	{
		{ 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, },
		{ 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, },
		{ 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, },
		{ 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3, },
	},
	{
		{ 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, },
		{ 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, },
		{ 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, },
		{ 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13, },
	},
	{
		{ 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, },
		{ 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, },
		{ 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, },
		{ 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12, },
	},
	{
		{ 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, },
		{ 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, },
		{ 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, },
		{ 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11, },
	}
};

void printb(unsigned long x){
	unsigned long m = 1;
	m=m<<63;
	for (int i=0; i<64; i++){
		if ((m & x) == 0){
			printf("0");
		} else {
			printf("1");
		}
		x=x<<1;
	}
	printf("\n");

}

int rot[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

unsigned long subkeys[16];

unsigned long rotate(unsigned long input, int n, int lim){
	unsigned long mask = 0;
	mask--;
	mask = mask << (sizeof(long)*8-lim);
	unsigned long ret = input << n;
	ret = ret | (input >> (lim-n));
	ret = ret & mask;
	return ret;
}

unsigned int getb(unsigned long input, unsigned int n){
	unsigned long mask = 1;
	mask = mask << (63-n);
	mask = input & (mask);
	return (mask!=0);
}

unsigned long setb(unsigned long input, unsigned int n, unsigned int v){
	unsigned long mask = 1;
	mask = mask << (63-n);
	unsigned long ret = input & (~mask);
	mask = mask * (v!=0);
	ret = mask + ret;
	return ret;
}

unsigned long map(unsigned long input, int map[], int n){
	unsigned long ret = 0;
	for (int i=0; i<n; i++){
		ret = setb(ret, i, getb(input, map[i]-1));
	}
	return ret;
}

unsigned long feistel(unsigned long input, unsigned long subkey){
	unsigned long expanded = map(input, e, 48);
	unsigned long xored = (expanded^subkey);
	unsigned long seb = 0;
	for (int i=0; i<8; i++){
		unsigned long temp = (xored << 6*i)>>(64-6);
		unsigned long temp2 = sub(temp, s[i])<<(64-4*(i+1));
		seb+=temp2;
	}
	return map(seb, p, 32);
}

void keyGen(unsigned long x){
	unsigned long l = map(x, pc1l, 28);
	unsigned long r = map(x, pc1r, 28);
	unsigned long temp;
	for (int i=0; i<16; i++){
		temp = 0;
		l = rotate(l, rot[i], 28);
		r = rotate(r, rot[i], 28);
		temp = l+ (r>>28);
		subkeys[i] = map(temp, pc2, 48);
	}
}

unsigned long des(unsigned long input, unsigned long key){
	unsigned long iped = map(input, ip, 64);
	keyGen(key);
	unsigned long inp = iped;
	unsigned long out;
	for (int i=0; i<16; i++){
		out=(inp<<32);
		out+=((feistel(inp<<32, subkeys[i])^(inp))>>32);
		inp = out;
	}
	out = (inp<<32) + (inp>>32);
	return map(out, fp, 64);
}

unsigned long undes(unsigned long input, unsigned long key){
	unsigned long iped = map(input, ip, 64);
	keyGen(key);
	unsigned long inp = iped;
	unsigned long out;
	for (int i=0; i<16; i++){
		out=(inp<<32);
		out+=((feistel(inp<<32, subkeys[16-i-1])^(inp))>>32);
		inp = out;
	}
	out = (inp<<32) + (inp>>32);
	return map(out, fp, 64);
}

unsigned long str2bin(char s[8]){
	unsigned long x = 0;
	for (int i=0; i<8; i++){
		x=x<<8;
		x+=s[i];
	}
	return x;
}

char* getPlainText(){
	char *temp = NULL;
	size_t n;
	getline(&temp, &n, stdin);
	int len = strlen(temp);
	temp[len-1]='\0';
	temp = realloc(temp, len*sizeof(char));
	return temp;
}

char** splitstr(char* str){
	int len = strlen(str)/8+(strlen(str)%8!=0);
	char **arr = malloc(len+1);
	for (int i=0; i<len; i++){
		arr[i]=malloc(8);
		strncpy(arr[i], str+8*i, 8);
	}
	arr[len]=NULL;
	return arr;
}

unsigned long* strarrtolongarr(char **arr){
	int len;
	for (len=0; arr[len]!=NULL; len++);
	unsigned long* larr = malloc(sizeof(unsigned long)*len+1);
	for (int i=0; i<len; i++){
		larr[i]=str2bin(arr[i]);
	}
	return larr;
}

int main(){
	char* plaintext = getPlainText();
	char** textarr = splitstr(plaintext);
	free(plaintext);
	unsigned long* longarr = strarrtolongarr(textarr);
	for(;textarr!=NULL;textarr++) free(*textarr);
	printf("%lx\n",x);
	printb(x);
	char sk[] = "aaaaaaaa";
	unsigned long k = str2bin(sk);
	printb(k);
	unsigned long ciphertext = des(x,k);
	printb(ciphertext);
	printf("%lx\n",ciphertext);
	unsigned long decoded = undes(ciphertext,k);
	printf("%lx\n",decoded);
}
