#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"
#include "OakShared.pb-c.h"
#include "FileTranslator.h"

#define FILESIZE_LIMIT 9000000

extern struct Save save_t;

int main(int argc, char** argv) {
	printf("CSAV001CNV Save to Protocol Buffer Data Translation Program\n");
	if(argc < 3) {
		fprintf(stderr, "CSAV001ABN Incorrect number of arguments: input.proto, original.sav\n");
		exit(1);
	}

	FILE* inFile = fopen(argv[1], "r");
	if(inFile == NULL) {
		fprintf(stderr, "CSAV001ABN Failed to open input file.\n");
		exit(1);
	}
	char* outFileName = strcat(argv[1], ".edited.sav");
	printf("CSAV00200I Output file = %s\n", outFileName);

	FILE* origFile = fopen(argv[2], "r");
	if(origFile == NULL) {
		fprintf(stderr, "CSAV001ABN Failed to open original file.\n");
		exit(1);
	}

	FILE* outFile = fopen(outFileName, "w");
	if(outFile == NULL) {
		fprintf(stderr, "CSAV001ABN Failed to open output file (read-only file system or bad DDNAME?)\n"),
		exit(1);
	}

	uint8_t* protoData;
	fseek(inFile, 0, SEEK_END);
	int inFileLen = ftell(inFile);
	fseek(inFile, 0, SEEK_SET);
	protoData = malloc(inFileLen);
	if(!protoData) {
		printf("CSAV001ABD OUT OF MEMORY.\n");
		exit(420);
	}
	fread(protoData, inFileLen, sizeof(uint8_t), inFile);
	fclose(inFile);
	printf("CSAV001CNV Input protobuf file length: %d\n", inFileLen);
	writeSave(origFile, outFile, protoData, inFileLen);
	printf("CSAV001CNV Execution complete\n");
	return 0;
}

