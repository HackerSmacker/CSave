#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"


// de-XOR the save file.
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


// de-XOR an item serial.
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


// Print out info about a serial to the console.
void dumpSerial(ProtobufCBinaryData item_serial_number) {
	// The actual data and the length.
	uint8_t* data = item_serial_number.data;
	int len = item_serial_number.len;
	int i;
	// Print out the data, char by char.
	printf("CSAV001INV Parsing serial ");
	for(i = 0; i < len; i++) {
		printf("%x", data[i]);
	}
	printf("\n");
	// Done printing; get the magic number and make sure it's 1 - removed because this is dumb
	uint8_t magicNumber = data[0];
	// Do some shifts to obtain the XOR seed. Keep in mind this code won't work on big-endian machines (so no SGI machines).
	uint32_t xorSeed = data[1] | (data[2] << 8) | (data[3] << 8) | (data[4] << 8);
	printf("CSAV001INV XOR seed for this item: %x\n", xorSeed);
	// Take off 5 bytes for the magic number and seed, the rest is the XOR'd data.
	int xorDataLen = len - 5;
	uint8_t* xorData = malloc(len - 5);
	// Copy the rest of the serial to the buffer so it can be de-XOR'd.
	memcpy(xorData, data + 5, xorDataLen);
	// Call the un-XOR function.
	uint8_t* decryptedData = xorItemSerialData(xorData, xorSeed, xorDataLen);
	// Print out the encrypted and decrypted data
	printf("CSAV001INV Payload length: %d\n", xorDataLen);
	printf("CSAV001INV Data encrypted: ");
	for(i = 0; i < xorDataLen; i++) {
		printf("%x", xorData[i]);
	}
	printf("\n");
	printf("CSAV001INV Data decrypted: ");
	for(i = 0; i < xorDataLen; i++) {
		printf("%x", decryptedData[i]);
	}
	printf("\n");

}

// Convert the InventoryBalanceData from the serial into the full path.
uint8_t* serialIBDToPath(uint8_t* ibd) {
	return NULL;
}
