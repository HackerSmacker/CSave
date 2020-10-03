#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "FileTranslator.h"

#define FILESIZE_LIMIT 2000000

int i;
int j;
int loopContinue = 1;
uint8_t fileBuffer[FILESIZE_LIMIT];
uint8_t* outBuffer;
OakSave__Character* charData;

int main(int argc, char** argv) {
	if(argc < 3) {
		fprintf(stderr, "CSAVAB050A Missing filenames (specify input file and output file)\n");
		exit(1);
	}
// Open input and output files
	FILE* inFile;
	FILE* outFile;
	inFile = fopen(argv[1], "r");
	outFile = fopen(argv[2], "w");
	if(inFile == NULL) {
		fprintf(stderr, "CSAVAB001A Failed to open input file\n");
		exit(1);
	}
	if(outFile == NULL) {
		fprintf(stderr, "CSAVAB001A Failed to open output file\n");
		exit(1);
	}
	int fileLen = read_buffer(FILESIZE_LIMIT, fileBuffer, inFile);

// Print the welcome message
	printf("CSAV00100I Borderlands 3 CSave\n");
	printf("CSAV00100I by HackerSmacker\n");
	printf("CSAB00100I THIS IS NOT FREE SOFTWARE AT THE MOMENT.\n");
	printf("CSAV00100S Save file size = %d\n", fileLen);

// Define the object, and read the data from the buffer (i.e. unpack it)
	charData = oak_save__character__unpack(NULL, fileLen, fileBuffer);

// Get some basic info about the save file.
	printf("CSAV001GEN Name: %s\n", charData->preferred_character_name);	
	printf("CSAV001GEN Save game identifier: %lu\n", charData->save_game_id);
	printf("CSAV001GEN Last save timestamp: %lu\n", charData->last_save_timestamp);
	printf("CSAV001GEN Total time played, in seconds: %lu\n", charData->time_played_seconds);
	printf("CSAV001GEN Playthroughs completed: %d\n", charData->playthroughs_completed);
	printf("CSAV001GEN Save GUID: %s\n", charData->save_game_guid);

// -------------------- Immediate mode main loop ---------------------
	char* command;
	command = malloc(1024);
	printf("CSAV001IMM Execution begins...\n");
	while (loopContinue == 1) {
		printf("*Go\n");
		fgets(command, 1024, stdin);
		if((strcmp("quit\n", command) == 0) | (strcmp("exit\n", command) == 0)) {
			printf("CSAV001IMM Done\n");
			break;
		}
		else if(strcmp("help\n", command) == 0) {
			printf("CSAV001IMM I guess you could say there's no help for the wicked\n");
		}
		else if(strcmp("set name\n", command) == 0) {
			printf("CSAV001IMM Specify new name now...\n");
			printf("*Input\n");
			fgets(command, 1024, stdin);
			strcpy(charData->preferred_character_name, command);
			printf("CSAV001IMM Name set complete\n");
		}
		else if(strcmp("set mayhemlevel\n", command) == 0) {
			uint32_t newMHLevel;
			printf("CSAV001IMM Specify new Mayhem level now (0-10)...\n");
			printf("*Input\n");
			fgets(command, 1024, stdin);
			newMHLevel = atoi(command);
			charData->mayhem_level = newMHLevel;
			printf("CSAV001IMM Mayhem level updated\n");
		}
		else if(strcmp("set expoints\n", command) == 0) {
			int32_t newEXPointsCount;
			printf("CSAV001IMM Specify number of experience points...\n");
			printf("*Input\n");
			fgets(command, 1024, stdin);
			newEXPointsCount = atoi(command);
			charData->experience_points = newEXPointsCount;
			printf("CSAV001IMM Experience point counter updated\n");
		}
		else if(strcmp("set level\n", command) == 0) {
			int xpLevel;
			printf("CSAV001IMM Enter new level\n");
			printf("*Input\n");
			fgets(command, 1024, stdin);
			xpLevel = atoi(command);
			charData->experience_points = levelToXP[xpLevel];
			printf("CSAV001IMM Level set and XP counter updated\n");
		}
	}

// -------------------------- End main loop --------------------------


// Get packed size, allocate output buffer, pack the protobuf, and write it to a file
	size_t writeLen = oak_save__character__get_packed_size(charData);
	outBuffer = malloc(writeLen);
	oak_save__character__pack(charData, outBuffer);
	printf("CSAV001END Writing output file...\n");
	fwrite(outBuffer, writeLen, 1, outFile);
	free(outBuffer);
	return 0;
}
