#include <stdio.h>
#include <stdlib.h>
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "FileTranslator.h"
#include "ItemCode.h"

#define FILESIZE_LIMIT 9000000

int i;
int j;

uint8_t fileBuffer[FILESIZE_LIMIT];
OakSave__Profile* profData;

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("CSAV001ABD Missing filename\n");
		exit(1);
	}
// Read the file
	FILE* pbF;
	pbF = fopen(argv[1], "r");
	if(pbF == NULL) {
		printf("CSAV001ABD Failed to open file\n");
		exit(1);
	}
	int fileLen = read_buffer(FILESIZE_LIMIT, fileBuffer, pbF);

// Print the welcome message
	printf("CSAV00100I Borderlands 3 CSave\n");
	printf("CSAV00100I by HackerSmacker\n");
	printf("CSAV00100S Profile file size = %d\n", fileLen);

// Define the object, and read the data from the buffer (i.e. unpack it)
	profData = oak_save__profile__unpack(NULL, fileLen, fileBuffer);

// print it out!
	printf("CSAV001GEN Last used save ID: %d\n", profData->last_used_savegame_id);
	printf("CSAV001GEN Difficulty: %d\n", profData->difficulty);
	printf("CSAV001GEN Matchmaking region: %s\n", profData->matchmaking_region);
	printf("CSAV001GEN Streaming service: %d\n", profData->streaming_service);

// Profile Stats
	printf("CSAV001PSD Profile stat data:\n");
	OakSave__GameStatSaveGameData** profile_stats_data = profData->profile_stats_data;
	size_t n_profile_stats_data = profData->n_profile_stats_data;
	for(i = 0; i < n_profile_stats_data; i++) {
		printf("CSAV001PSD Stat: %s\n", profile_stats_data[i]->stat_path);
		printf("CSAV001PSD Value: %d\n", profile_stats_data[i]->stat_value);
	}

// ICL in the profile?
	OakSave__InventoryCategorySaveData** bank_inventory_category_list = profData->bank_inventory_category_list;
	size_t n_bank_inventory_category_list = profData->n_bank_inventory_category_list;
	for(i = 0; i < n_bank_inventory_category_list; i++) {
		printf("CSAV001ICL Inventory Category %d is of type %x and has value %d\n", i, bank_inventory_category_list[i]->base_category_definition_hash, bank_inventory_category_list[i]->quantity);
	}



	oak_save__profile__free_unpacked(profData, NULL);
	return 0;
}
