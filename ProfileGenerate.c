#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "OakShared.pb-c.h"
#include "OakProfile.pb-c.h"
#include "OakSave.pb-c.h"
#include "FileTranslator.h"
#include "HelperVars.h"
#include "Customizations.h"

#define FILESIZE_LIMIT 9000000

int i;
int j;
int k;
int loopContinue = 1;
uint8_t fileBuffer[FILESIZE_LIMIT];
uint8_t* outBuffer;
OakSave__Profile* profData;

int main(int argc, char** argv) {
	if(argc < 3) {
		printf("CSAV001ABD Missing filenames (specify input file and output file)\n");
		exit(1);
	}
	if(strcmp(argv[1], argv[2]) == 0) {
		printf("CSAV001ABD The input and output files cannot be the same.\n");
		exit(1);
	}

// Open input and output files
	FILE* inFile;
	FILE* outFile;
	inFile = fopen(argv[1], "r");
	outFile = fopen(argv[2], "w");
	if(inFile == NULL) {
		printf("CSAV001ABD Failed to open input file\n");
		exit(1);
	}
	if(outFile == NULL) {
		printf("CSAV001ABD Failed to open output file\n");
		exit(1);
	}
	int fileLen = read_buffer(FILESIZE_LIMIT, fileBuffer, inFile);

// Print the welcome message
	printf("CSAV00100I Borderlands 3 CSave\n");
	printf("CSAV00100I by HackerSmacker\n");
	printf("CSAV00100S Save file size = %d\n", fileLen);

// Define the object, and read the data from the buffer (i.e. unpack it)
	profData = oak_save__profile__unpack(NULL, fileLen, fileBuffer);

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
		else if(strcmp("set goldenkeys\n", command) == 0) {
			OakSave__InventoryCategorySaveData** icl = profData->bank_inventory_category_list;
			int numICL = profData->n_bank_inventory_category_list;
			printf("CSAV001IMM How many golden keys do you want?\n");
			printf("*Input\n");
			fgets(command, 1024, stdin);
			int32_t newMoney = atoi(command);
			for(i = 0; i < numICL; i++) {
				if(icl[i]->base_category_definition_hash == currencyHashes[2]) {
					icl[i]->quantity = newMoney;
				}
			}
			printf("CSAV001IMM Golden keys updated\n");
		}
		else if(strcmp("unlock all\n", command) == 0) {
			printf("CSAV001IMM Have you legit purchased all cosmetics?\n*Input\n");
			fgets(command, 1024, stdin);
			if(command[0] == 'y') {
				printf("CSAV001IMM Unlocking all customizations...\n");
				// 0. fool save file
				profData->n_unlocked_customizations = numAllAssets;
				// 1. allocate space for everything
				OakSave__OakCustomizationSaveGameData* custs = malloc(sizeof(OakSave__OakCustomizationSaveGameData) * numAllAssets);
				// 2. copy them in
				for(i = 0; i < numAllAssets; i++) {
					custs[0].customization_asset_path = allAssetPaths[i];
				}
				// 3. store value
				//profData->unlocked_customizations = custs;
			}
		}
		else {
			printf("CSAV001IMM Invalid command...\n");
		}
	}

// -------------------------- End main loop --------------------------


// Get packed size, allocate output buffer, pack the protobuf, and write it to a file
	size_t writeLen = oak_save__profile__get_packed_size(profData);
	outBuffer = malloc(writeLen);
	oak_save__profile__pack(profData, outBuffer);
	printf("CSAV001END Writing output file...\n");
	fwrite(outBuffer, writeLen, 1, outFile);
	free(outBuffer);
	return 0;
}
