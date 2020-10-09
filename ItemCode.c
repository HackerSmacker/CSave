#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"

uint8_t* xorData(uint8_t* data, uint8_t seed) {
	int dataLen = strlen(data);
	uint8_t* out;
	out = malloc(dataLen);
	if(seed == 0) {
		strcpy(out, data);
	}
	int i;
	uint8_t currentByte = 0;
	for(i = 0; i < dataLen; i++) {
		currentByte = (seed >> 5) & 0xFFFFFFFF;
		currentByte = ((currentByte * 0x10A860C1) % 0xFFFFFFFB);
		out[i] = ((data[i] ^ currentByte) & 0xFF);
	}
	return out;
}





void dumpSerial(uint8_t* serial) {
	printf("CSAV001INV Parsing serial %x\n", serial);
}
