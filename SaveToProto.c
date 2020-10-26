#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"
#include "OakShared.pb-c.h"
#include "FileTranslator.h"

#define FILESIZE_LIMIT 2000000

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

	//uint8_t inputBuffer[FILESIZE_LIMIT];
	//int fileLen = read_buffer(FILESIZE_LIMIT, inputBuffer, inFile);
	//rewind(inFile); // Rewind the tape drive like it's 1981
	//printf("CSAV00200I File size = %d\n", fileLen);
	readSave(inFile);
	return 0;
}

