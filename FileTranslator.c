#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
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

void readSave(uint8_t* save, FILE* file) {
	// The actual save.
	struct Save save_t;
	// Pointer that holds the position in the file.
	int pos = 0;
	// Allocate some space to hold "GVAS"
/* CRUSTY CODE BEGINS
	save_t.header = malloc(4);
	// Copy in "GVAS"
	printf("%d\n", pos);
	memcpy(save_t.header, save, 4);
	// Increment the counter
	pos += 3;
	printf("%d\n", pos);
	// Memory for the save data
	char sgVersion[4];
	char pkgVersion[4];
	char major[2];
	char minor[2];
	char patch[2];
	char build[4];
	// Read the save
	memcpy(sgVersion, save + pos, 4);
	printf("%d\n", pos);
	pos += 4;
	memcpy(pkgVersion, save + pos, 4);
	printf("%d\n", pos);
	pos += 4;
	memcpy(major, save + pos, 2);
	printf("%d\n", pos);
	pos += 2;
	memcpy(minor, save + pos, 2);
	printf("%d\n", pos);
	pos += 2;
	memcpy(patch, save + pos, 2);
	printf("%d\n", pos);
	pos += 2;
	memcpy(build, save + pos, 4);
	printf("%d\n", pos);
	pos += 4;
	printf("%d,%d,%d,%d,%d,%d\n", sgVersion, pkgVersion, major, minor, patch, build);
	// Convert and put in the struct
	save_t.sg_version = strtol(sgVersion, NULL, 10);
	save_t.pkg_version = strtol(pkgVersion, NULL, 10);
	save_t.engine_major = strtol(major, NULL, 10);
	save_t.engine_minor = strtol(minor, NULL, 10);
	save_t.engine_patch = strtol(patch, NULL, 10);
	save_t.engine_build = strtol(build, NULL, 10);
END CRUSTY CODE */
	fscanf(file, "%4c", save_t.header);
	fscanf(file, "%" SCNd32, save_t.sg_version);
	fscanf(file, "%" SCNd32, save_t.pkg_version);
	printf("CSAV001RWS Save information:\n");
	printf("Header: %s\n", save_t.header);
	printf("SG version: %" SCNd32 "\n", save_t.sg_version);
	printf("Package version: %" SCNd32 "\n", save_t.pkg_version);
	//printf("Engine version: %d.%d.%d\n", save_t.engine_major, save_t.engine_minor, save_t.engine_patch);
	//printf("Engine build: %d\n", save_t.engine_build);
}
