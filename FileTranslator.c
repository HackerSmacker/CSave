#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FileTranslator.h"


size_t read_buffer(int max_length, uint8_t* out, FILE* f) {
	size_t cur_len = 0;
	size_t nread;
	while ((nread = fread(out + cur_len, 1, max_length - cur_len, f)) != 0) {
		cur_len += nread;
		if (cur_len == max_length) {
			fprintf(stderr, "CSAVAB100B Buffer exceeded!\n");
			exit(1);
		}
	}
	return cur_len;
}

char PrefixMagic[] = {
	0x71, 0x34, 0x36, 0xB3, 0x56, 0x63, 0x25, 0x5F,
	0xEA, 0xE2, 0x83, 0x73, 0xF4, 0x98, 0xB8, 0x18,
	0x2E, 0xE5, 0x42, 0x2E, 0x50, 0xA2, 0x0F, 0x49,
	0x87, 0x24, 0xE6, 0x65, 0x9A, 0xF0, 0x7C, 0xD7
};

char XorMagic[] = {
	0x7C, 0x07, 0x69, 0x83, 0x31, 0x7E, 0x0C, 0x82,
	0x5F, 0x2E, 0x36, 0x7F, 0x76, 0xB4, 0xA2, 0x71,
	0x38, 0x2B, 0x6E, 0x87, 0x39, 0x05, 0x02, 0xC6,
	0xCD, 0xD8, 0xB1, 0xCC, 0xA1, 0x33, 0xF9, 0xB6
};

size_t decryptSave(uint8_t* buffer, int offset, int length) {
	if(buffer == NULL) {
		printf("CSAV001RWS Save decryption error: null encountered.\n");
		return -1;
	}
	else if(offset < 0) {
		printf("CSAV001RWS Caller specified a negative offset for some reason.\n");
		return -1;
	}
	else if(length < 0) {
		printf("CSAV001RWS Caller specified a negative length for some reason.\n");
		return -1;
	}
	else if(offset > strlen(buffer)) {
		printf("CSAV001RWS Offset specified was larger than strlen's measure of the buffer length.\n");
		return -1;
	}
	else if(offset + length > strlen(buffer)) {
		printf("CSAV001RWS Offset plus length was larger than strlen(buffer).\n");
		return -1;
	}
	else if(length == 0) {
		printf("CSAV001RWS Offset was zero. Save not processed.\n");
		return -1;
	}
	int i;
	int o;
	o = offset + 1;
	for(i = length - 1; i >= 0; i--) {
		char b = i < 32 ? PrefixMagic[i] : buffer[o - 32];
		b ^= XorMagic[o % 32];
		buffer[o] ^= b;
		o--;
	}
	return length;
}

