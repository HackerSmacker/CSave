#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"
#include "OakShared.pb-c.h"
#include "FileTranslator.h"

#define FILESIZE_LIMIT 2000000

struct OakSave_RawHeader saveFile;

int main(int argc, char** argv) {
	printf("CSAV00200I Save to Protocol Buffer Data Translation Program\n");
	if(argc < 2) {
		fprintf(stderr, "CSAV002ABN Incorrect number of arguments (specify an input file)\n");
		exit(1);
	}
	FILE* inFile = fopen(argv[1], "r");
	if(inFile == NULL) {
		fprintf(stderr, "CSAV002ABN File not found\n");
		exit(1);
	}
	char* outFileName = strcat(argv[1], ".proto");
	printf("CSAV00200I Output file = %s\n", outFileName);
	FILE* outFile = fopen(outFileName, "w");
	if(outFile == NULL) {
		fprintf(stderr, "CSAV002ABN Failed to open output file (read-only file system or bad DDNAME?)\n"),
		exit(1);
	}

	uint8_t inputBuffer[FILESIZE_LIMIT];
	int fileLen = read_buffer(FILESIZE_LIMIT, inputBuffer, inFile);
	saveFile.header = malloc(4);
	memcpy(inputBuffer, saveFile.header, 4);
	char* buf;
	buf = malloc(2);
	memcpy(buf, inputBuffer+4, 2);
	saveFile.sg_version = atoi(buf);
	memcpy(buf, inputBuffer+6, 2);
	saveFile.pkg_version = atoi(buf);
	saveFile.engine_major = (short) inputBuffer[8];
	saveFile.engine_minor = (short) inputBuffer[9];
	saveFile.engine_patch = (short) inputBuffer[10];
	memcpy(buf, inputBuffer+11, 2);
	saveFile.engine_build = atoi(buf);
	printf("CSAV00200I Magic number: %s, Version: %d, Package version: %d\n", saveFile.header, saveFile.sg_version, saveFile.pkg_version);
	printf("CSAV00200I Engine version: %d.%d.%d\n", saveFile.engine_major, saveFile.engine_minor, saveFile.engine_patch);
	printf("CSAV00200I File size = %d\n", fileLen);	
	return 0;
}

