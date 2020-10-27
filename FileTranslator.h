#ifndef FILETRANS_H
#define FILETRANS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

size_t read_buffer(int max_length, uint8_t* out, FILE* f);

struct Save {
	char* header;
	int32_t sg_version;
	int32_t pkg_version;
	int16_t engine_major;
	int16_t engine_minor;
	int16_t engine_patch;
	uint32_t engine_build;
	char* build_id;
	int build_id_length;
	int32_t fmt_version;
	int32_t fmt_count;
	char** custom_format_data; // DON'T use, use struct keyValuePair
	int32_t sg_type_len;
	char* sg_type;
	int32_t remaining_data_len;
	char* remaining_data;
};

struct keyValuePair {
	char* guid;
	int entry;
};

void readSave(FILE* file);

#endif


