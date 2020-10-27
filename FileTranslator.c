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
	/*
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
	*/
	int i;
	//int o;
	//o = offset + 1;
	//for(i = length - 1; i >= 0; i--) {
	//	char b = i < 32 ? PrefixMagic[i] : buffer[o - 32];
	//	b ^= XorMagic[o % 32];
	//	buffer[o] ^= b;
	//	o--;
	//}
	char b;
	for(i = length - 1; i >= 0; i--) {
		if(i < 32) {
			b = PrefixMagic[i];
		}
		else {
			b = buffer[i - 32];
		}
		b ^= XorMagic[i % 32];
		buffer[i] ^= b;
	}
	return length;
}

struct Save save_t;

void readSave(FILE* file) {
	printf("CSAV001RWS Reading file...\n");
	// The actual save.
	// Read it byte by byte.
	printf("%d, ", ftell(file));
	save_t.header = malloc(4);
	fread(save_t.header, sizeof(char), 4, file);
	printf("%d, ", ftell(file));
	fread(&save_t.sg_version, sizeof(int32_t), 1, file);
	printf("%d, ", ftell(file));
	fread(&save_t.pkg_version, sizeof(int32_t), 1, file);
	printf("%d, ", ftell(file));
	fread(&save_t.engine_major, sizeof(int16_t), 1, file);
	printf("%d, ", ftell(file));
	fread(&save_t.engine_minor, sizeof(int16_t), 1, file);
	printf("%d, ", ftell(file));
	fread(&save_t.engine_patch, sizeof(int16_t), 1, file);
	printf("%d, ", ftell(file));
	fread(&save_t.engine_build, sizeof(uint32_t), 1, file);
	printf("%d, ", ftell(file));
	fread(&save_t.build_id_length, sizeof(int32_t), 1, file);
	printf("%d, ", ftell(file));
	save_t.build_id = malloc(save_t.build_id_length);
	fread(save_t.build_id, sizeof(char), save_t.build_id_length, file);
	printf("%d, ", ftell(file));
	fread(&save_t.fmt_version, sizeof(int32_t), 1, file);
	printf("%d, ", ftell(file));
	fread(&save_t.fmt_count, sizeof(int32_t), 1, file);
	printf("%d, ", ftell(file));
	struct keyValuePair* kvp_t;
	kvp_t = malloc(save_t.fmt_count * sizeof(struct keyValuePair));
	int i;
	for(i = 0; i < save_t.fmt_count; i++) {
		kvp_t[i].guid = malloc(16);
		fread(kvp_t[i].guid, sizeof(char), 16, file);
		printf("%d, ", ftell(file));
		fread(&kvp_t[i].entry, sizeof(int32_t), 1, file);
		printf("%d, ", ftell(file));
	}
	printf("%d, ", ftell(file));
	fread(&save_t.sg_type_len, sizeof(int32_t), 1, file);
	save_t.sg_type = malloc(save_t.sg_type_len);
	fread(save_t.sg_type, sizeof(char), save_t.sg_type_len, file);
	fread(&save_t.remaining_data_len, sizeof(int32_t), 1, file);
	int payloadStart = ftell(file);
	save_t.remaining_data = malloc(save_t.remaining_data_len);
	fread(save_t.remaining_data, sizeof(char), save_t.remaining_data_len, file);

	printf("CSAV001RWS Save information:\n");
	printf("Header: %s\n", save_t.header);
	printf("SG version: %" SCNd32 "\n", save_t.sg_version);
	printf("Package version: %d\n", save_t.pkg_version);
	printf("Engine version: %d.%d.%d\n", save_t.engine_major, save_t.engine_minor, save_t.engine_patch);
	printf("Build ID length: %d\n", save_t.build_id_length);
	printf("Build ID: %s\n", save_t.build_id);
	printf("Custom format version: %d\n", save_t.fmt_version);
	printf("Custom format count: %d\n", save_t.fmt_count);
	//for(i = 0; i < save_t.fmt_count; i++) {
	//	printf("Custom format %d: GUID: %x, entry %d\n", i, kvp_t[i].guid, kvp_t[i].entry);
	//}
	printf("Save type length: %d\n", save_t.sg_type_len);
	printf("Save type: %s\n", save_t.sg_type);
	printf("Payload start position: %d\n", payloadStart);
	printf("Payload length: %d\n", save_t.remaining_data_len);

	int processedLen = decryptSave(save_t.remaining_data, 0, save_t.remaining_data_len);
}
