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

OakSave__CrewQuartersRoomItemSaveGameData** unlocked_crew_quarters_rooms;
size_t n_unlocked_crew_quarters_rooms;
OakSave__CrewQuartersDecorationItemSaveGameData** unlocked_crew_quarters_decorations;
size_t n_unlocked_crew_quarters_decorations;
OakSave__OakInventoryCustomizationPartInfo** unlocked_inventory_customization_parts;
size_t n_unlocked_inventory_customization_parts;
OakSave__OakCustomizationSaveGameData** unlocked_customizations;
size_t n_unlocked_customizations;
OakSave__OakSDUSaveGameData** profile_sdu_list;
size_t n_profile_sdu_list;
size_t n_bank_inventory_category_list;
OakSave__InventoryCategorySaveData** bank_inventory_category_list;
OakSave__GameStatSaveGameData** profile_stats_data;
size_t n_profile_stats_data;
int fileLen;
FILE* pbF;

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("CSAV001ABD Missing filename\n");
		exit(1);
	}
// Read the file
	pbF = fopen(argv[1], "r");
	if(pbF == NULL) {
		printf("CSAV001ABD Failed to open file\n");
		exit(1);
	}
	fileLen = read_buffer(FILESIZE_LIMIT, fileBuffer, pbF);

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
	profile_stats_data = profData->profile_stats_data;
	n_profile_stats_data = profData->n_profile_stats_data;
	for(i = 0; i < n_profile_stats_data; i++) {
		printf("CSAV001PSD Stat: %s\n", profile_stats_data[i]->stat_path);
		printf("CSAV001PSD Value: %d\n", profile_stats_data[i]->stat_value);
	}

// ICL in the profile?
	bank_inventory_category_list = profData->bank_inventory_category_list;
	n_bank_inventory_category_list = profData->n_bank_inventory_category_list;
	for(i = 0; i < n_bank_inventory_category_list; i++) {
		printf("CSAV001ICL Inventory Category %d is of type %x and has value %d\n", i, bank_inventory_category_list[i]->base_category_definition_hash, bank_inventory_category_list[i]->quantity);
	}

// SDUs in the profile
	n_profile_sdu_list = profData->n_profile_sdu_list;
	profile_sdu_list = profData->profile_sdu_list;
	for(i = 0; i < n_profile_sdu_list; i++) {
		printf("CSAV001SDU %d SDUs, of type %s\n", profile_sdu_list[i]->sdu_level, profile_sdu_list[i]->sdu_data_path);
	}

// Customizations	
	n_unlocked_customizations = profData->n_unlocked_customizations;
	unlocked_customizations = profData->unlocked_customizations;
	for(i = 0; i < n_unlocked_customizations; i++) {
		printf("CSAV001CST Customization %d: %s\n", i, unlocked_customizations[i]->customization_asset_path);
	}


// Inventory customizations
	n_unlocked_inventory_customization_parts = profData->n_unlocked_inventory_customization_parts;
	unlocked_inventory_customization_parts = profData->unlocked_inventory_customization_parts;
	for(i = 0; i < n_unlocked_inventory_customization_parts; i++) {
		printf("CSAV001CST Inventory Customization %d: hash %x\n", i, unlocked_inventory_customization_parts[i]->customization_part_hash);
	}

// Decorations
	n_unlocked_crew_quarters_decorations = profData->n_unlocked_crew_quarters_decorations;
	unlocked_crew_quarters_decorations = profData->unlocked_crew_quarters_decorations;
	for(i = 0; i < n_unlocked_crew_quarters_decorations; i++) {
		printf("CSAV001CST Decoration %d: %s\n", i, unlocked_crew_quarters_decorations[i]->decoration_item_asset_path);
	}


// Rooms
	n_unlocked_crew_quarters_rooms = profData->n_unlocked_crew_quarters_rooms;
	unlocked_crew_quarters_rooms = profData->unlocked_crew_quarters_rooms;

	oak_save__profile__free_unpacked(profData, NULL);
	return 0;
}
