#ifndef FILETRANS_H
#define FILETRANS_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

size_t read_buffer(int max_length, uint8_t* out, FILE* f);

struct OakSave_RawHeader {
	char* header;		// 0-3
	int sg_version;		// 4-5
	int pkg_version;	// 6-7
	short engine_major;	// 8
	short engine_minor;	// 9
	short engine_patch;	// 10
	int engine_build;	// 11-12
	char* build_id;		// 13
	int fmt_version;
	int fmt_count;
	char** custom_format_data;
};

#endif


