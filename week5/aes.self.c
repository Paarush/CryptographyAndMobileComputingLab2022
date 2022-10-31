#include <stdio.h>
#include <stdint.h>

#define BYTE(x) ((&((union row*)(&(x))))->byte)
#define ROW(x) ((&((union row*)(&(x))))->row)
#define MOD(x,y) ((((x)%(y))+(y))%(y))

union row {
	uint32_t row;
	uint8_t byte[4];
};

uint32_t rotR32(uint32_t x, int n){
	return (x >> n) | (x << (32-n));
}

uint32_t rotR8(uint32_t x, int n){
	return (x >> n) | (x << (8-n));
}

uint8_t gdouble(uint8_t x){
	return (x<<1)^((x>>7)*0x1B);
}

uint8_t gmult(uint8_t x, uint8_t y){
	uint8_t ret = 0;
	uint8_t temp = x;
	if (y%2==0){
		ret ^= temp;
	}
	y=y>>1;
	for (int i=1;i<8; i++){
		temp=gdouble(temp);
		if (y%2==0){
			ret ^= temp;
		}
		y=y>>1;
	}
	return ret;
}

void shiftBytes(uint32_t x[4], int dir){
	for (int i=0; i<4; i++){
		x[i]=rotR32(x[i], i*dir);
	}
}

void subBytes(uint8_t x[4][4], uint8_t sbox[256]){
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			x[i][j] = sbox[x[i][j]];
		}
	}
}

void mixCol(uint8_t x[4], uint8_t row[4], int dir){
	uint8_t y[4];
	for (int i=0; i<4; i++){
		y[i] = 0;
		y[i] ^= gmult(x[0], row[MOD(0+i*dir,4)]);
		y[i] ^= gmult(x[1], row[MOD(1+i*dir,4)]);
		y[i] ^= gmult(x[2], row[MOD(2+i*dir,4)]);
		y[i] ^= gmult(x[3], row[MOD(3+i*dir,4)]);
	}
	for (int i=0; i<4; i++){
		x[i]=y[i];
	}
}

void mixCols(uint8_t x[4][4], uint8_t row[4], int dir){
}
