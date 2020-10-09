#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"

uint8_t* xorSaveData(uint8_t* data, uint8_t seed) {
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


uint8_t* xorItemSerialData(uint8_t* data, uint32_t seed, int len) {
	uint32_t xorMaskItself = (seed >> 5) & 0xFFFFFFFF;
	uint8_t* temp = malloc(len);
	if(seed == 0) {
		return data;
	}
	int i;
	for(i = 0; i < len; i++) {
		xorMaskItself = (xorMaskItself * 0x10A860C1) % 0xFFFFFFFB;
		temp[i] = ((data[i] ^ xorMaskItself) & 0xFF);
	}
	return temp;
}


void dumpSerial(ProtobufCBinaryData item_serial_number) {
	uint8_t* data = item_serial_number.data;
	int len = item_serial_number.len;
	int i;
	printf("CSAV001INV Parsing serial ");
	for(i = 0; i < len; i++) {
		printf("%x", data[i]);
	}
	printf("\n");
	uint8_t magicNumber = data[0];
	uint32_t xorSeed = data[1] | (data[2] << 8) | (data[3] << 8) | (data[4] << 8);
	int xorDataLen = len - 5;
	uint8_t* xorData = malloc(len - 5);
	uint8_t* dataToBeXORed = malloc(xorDataLen);
	for(i = 0; i < xorDataLen; i++) {
		xorData[i] = data[i + 5];
	}
	uint8_t* decryptedData = xorItemSerialData(xorData, xorSeed, xorDataLen);
	printf("CSAV001INV Payload length: %d\n", xorDataLen);
	printf("CSAV001INV Data encrypted: ");
	for(i = 0; i < xorDataLen; i++) {
		printf("%x\n", xorData[i]);
	}
	printf("\n");
	printf("CSAV001INV Data decrypted: ");
	for(i = 0; i < xorDataLen; i++) {
		printf("%x\n", decryptedData[i]);
	}
	printf("\n");

}
