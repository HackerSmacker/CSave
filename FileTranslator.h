#ifndef FILETRANS_H
#define FILETRANS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

size_t read_buffer(int max_length, uint8_t* out, FILE* f);

struct Save {
	char* header;		// 4
	int32_t sg_version;	// 4
	int32_t pkg_version;	// 4
	int16_t engine_major;	// 2
	int16_t engine_minor;	// 2
	int16_t engine_patch;	// 2
	uint32_t engine_build;	// 4
	char* build_id;		// ?
	int build_id_length;	// Not actually in the header
	int32_t fmt_version;	// 4
	int32_t fmt_count;	// 4
	char** custom_format_data;	// ?
	char* sg_type;
};

void readSave(uint8_t* save, FILE* file);

#endif


