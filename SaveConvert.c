#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"
#include "OakShared.pb-c.h"
#include "FileTranslator.h"

#define FILESIZE_LIMIT 2000000

extern struct Save save_t;

int main(int argc, char** argv) {
	int pc_in = 0;
	int pc_out = 0;
	printf("CSAV001CNV Binary Save Conversion Program\n");
	printf("CSAV001CNV WARNING: THIS PROGRAM TENDS TO OUTPUT GARBAGE!\n");
	if(argc < 4) {
		printf("CSAV001ABN Incorrect number of arguments! SaveConvert in.sav out.sav platformcode_in platformcode_out\n");
		exit(1);
	}

	FILE* inFile = fopen(argv[1], "r");
	if(inFile == NULL) {
		printf("CSAV001ABN File not found\n");
		exit(1);
	}
	FILE* outFile = fopen(argv[2], "w");
	if(outFile == NULL) {
		printf("CSAV001ABN Failed to open output file (read-only file system or bad DDNAME?)\n"),
		exit(1);
	}
	pc_in = atoi(argv[3]);
	pc_out = atoi(argv[4]);
	readSave(inFile, pc_in);
	writeSave(inFile, outFile, save_t.remaining_data, save_t.remaining_data_len, pc_out);
	printf("CSAV001CNV Execution complete\n");
	return 0;
}

