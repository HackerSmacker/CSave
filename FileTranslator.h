#ifndef FILETRANS_H
#define FILETRANS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

size_t read_buffer(int max_length, uint8_t* out, FILE* f);

struct Save {
	char* header;		// 0-3
	int32_t sg_version;		// 4-5
	int32_t pkg_version;	// 6-7
	int16_t engine_major;	// 8
	int16_t engine_minor;	// 9
	int16_t engine_patch;	// 10
	uint32_t engine_build;	// 11-12
	char* build_id;		// 13
	int32_t fmt_version;
	int32_t fmt_count;
	char** custom_format_data;
	char* sg_type;
};

#endif


