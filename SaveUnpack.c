#include <stdio.h>
#include <stdlib.h>
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "FileTranslator.h"

#define FILESIZE_LIMIT 2000000

int i;
int j;

uint8_t fileBuffer[FILESIZE_LIMIT];
OakSave__Character* charData;

int main(int argc, char** argv) {
	if(argc < 2) {
		fprintf(stderr, "CSAVAB050A Missing filename\n");
		exit(1);
	}
// Read the file
	FILE* pbF;
	pbF = fopen(argv[1], "r");
	if(pbF == NULL) {
		fprintf(stderr, "CSAVAB001A Failed to open file\n");
		exit(1);
	}	
	int fileLen = read_buffer(FILESIZE_LIMIT, fileBuffer, pbF);

// Print the welcome message
	printf("CSAV00100I Borderlands 3 CSave\n");
	printf("CSAV00100I by HackerSmacker\n");
	printf("CSAV00100S Save file size = %d\n", fileLen);

// Define the object, and read the data from the buffer (i.e. unpack it)
	charData = oak_save__character__unpack(NULL, fileLen, fileBuffer);

// Get some basic info about the save file.
	printf("CSAV001GEN Name: %s\n", charData->preferred_character_name);	
	printf("CSAV001GEN Save game identifier: %lu\n", charData->save_game_id);
	printf("CSAV001GEN Last save timestamp: %lu\n", charData->last_save_timestamp);
	printf("CSAV001GEN Total time played, in seconds: %lu\n", charData->time_played_seconds);
	printf("CSAV001GEN Selected Mayhem level: %lu\n", charData->mayhem_level);
	printf("CSAV001GEN Playthroughs completed: %d\n", charData->playthroughs_completed);
	printf("CSAV001GEN Preferred group mode: %lu\n", charData->preferred_group_mode);
	printf("CSAV001GEN Save GUID: %s\n", charData->save_game_guid);
	printf("CSAV001GEN Fast travel machine you will spawn at: %s\n", charData->last_active_travel_station);
	printf("CSAV001GEN Showing the TVHM notification: %d\n", charData->show_new_playthrough_notification);	

// Get info about the player class
	printf("CSAV001CLS Player class path: %s\n", charData->player_class_data->player_class_path);

// Get info on the save slot
	OakSave__OakPlayerCharacterSlotSaveGameData* slotData = charData->character_slot_save_game_data;

// Get some skill data - iterate through it and print out each skill
	OakSave__OakPlayerAbilitySaveGameData* skillData;
	skillData = charData->ability_data;
	printf("CSAV001SKL Skill points: %d\n", skillData->ability_points);
	printf("CSAV001SKL XP points: %d\n", charData->experience_points);
	printf("CSAV001SKL Tree grade (unlock status): %lu\n", skillData->tree_grade);
	//printf("CSAV001SKL Number of active abilities: %d\n", skillData->n_ability_slot_list);
	//for(i = 0; i < skillData->n_ability_slot_list; i++) {
	//	printf("CSAV001SKL Ability %d: %s\n", i, skillData->ability_slot_list[i]->ability_class_path);
	//}
	printf("CSAV00103S Number of skills: %d\n", skillData->n_tree_item_list);
	for(i = 0; i < skillData->n_tree_item_list; i++) {
		printf("CSAV001SKL Skill %d: %s\n", i, skillData->tree_item_list[i]->item_asset_path);
	}

// Get info on SDUs
	OakSave__OakSDUSaveGameData** sduData = charData->sdu_list;
	int sduDataSize = charData->n_sdu_list;
	for(i = 0; i < sduDataSize; i++) {
		printf("CSAV001SDU %d SDUs, of type %s\n", sduData[i]->sdu_level, sduData[i]->sdu_data_path);
	}	

// Get info on vehicle parts
	char** unlockedCarParts = charData->vehicle_parts_unlocked;
	int unlockedCarPartsSize = charData->n_vehicle_parts_unlocked;
	for(i = 0; i < unlockedCarPartsSize; i++) {
		printf("CSAV001VEH Car part %d: %s\n", i, unlockedCarParts[i]);
	}
	OakSave__VehicleUnlockedSaveGameData** carsUnlocked = charData->vehicles_unlocked_data;
	int numCarsUnlocked = charData->n_vehicles_unlocked_data;
	for(i = 0; i < numCarsUnlocked; i++) {
		printf("CSAV001VEH Car %d, type %s | unlocked: %d\n", i, carsUnlocked[i]->asset_path, carsUnlocked[i]->just_unlocked);
	}

// Print out every quest
	/* DEPRECATED CODE - NOT CONDUCIVE TO WRITING SAVE FILES REALLY
	OakSave__MissionPlaythroughSaveGameData** questData = charData->mission_playthroughs_data;
	int questDataSize = charData->n_mission_playthroughs_data;
	for(i = 0; i < questDataSize; i++) {
		int questChunkSize = questData[i]->n_mission_list;
		for(j = 0; j < questChunkSize; j++) {
			printf("CSAV001QST Quest %d on playthrough %d: %s\n", j, i, questData[i]->mission_list[j]->mission_class_path);
		}
	}
	*/
	OakSave__MissionPlaythroughSaveGameData** missionData = charData->mission_playthroughs_data;
	int missionDataLen = charData->n_mission_playthroughs_data;
	int k;
	for(i = 0; i < missionDataLen; i++) {
		OakSave__MissionStatusPlayerSaveGameData** missionDataForPlaythrough = missionData[i]->mission_list;
		int missionDFPLen = missionData[i]->n_mission_list;
		for(j = 0; j < missionDFPLen; j++) {
			printf("CSAV001MSN Mission %d on playthrough %d: %s\n", j, i, missionDataForPlaythrough[j]->mission_class_path);
			printf("CSAV001MSN Current objective for mission %d: %s\n", j, missionDataForPlaythrough[j]->active_objective_set_path);
			printf("CSAV001MSN Objective completion status for mission %d: \n", j);
			printf("CSAV001MSN ");
			for(k = 0; k < missionDataForPlaythrough[j]->n_objectives_progress; k++) {
				printf("%d ", missionDataForPlaythrough[j]->objectives_progress[k]);
			}
			printf("\n");
		}
	}

// Dump out ammo stats
	OakSave__ResourcePoolSavegameData** resData = charData->resource_pools;
	int numResData = charData->n_resource_pools;
	for(i = 0; i < numResData; i++) {
		printf("CSAV001AMO Has %f of %s\n", resData[i]->amount, resData[i]->resource_path);
	}

// Guardian rank
	OakSave__GuardianRankCharacterSaveGameData* grData = charData->guardian_rank_character_data;
	printf("CSAV001GRD Guardian tokens: %lu\n", grData->guardian_available_tokens);
	printf("CSAV001GRD Guardian rank: %lu\n", grData->guardian_rank);
	printf("CSAV001GRD Guardian experience: %lu\n", grData->guardian_experience);
	printf("CSAV001GRD Are guardian ranks enabled? %d\n", grData->is_rank_system_enabled);
	printf("CSAV001GRD Guardian rank seed: %lu\n", grData->guardian_reward_random_seed);
	int grRewardsNum = grData->n_rank_rewards;
	OakSave__GuardianRankRewardCharacterSaveGameData** grRewards = grData->rank_rewards;
	for(i = 0; i < grRewardsNum; i++) {
		printf("CSAV001GRD Guardian reward %d has %d tokens in it and is %s\n", i, grRewards[i]->num_tokens, grRewards[i]->reward_data_path);
	}
	OakSave__GuardianRankPerkCharacterSaveGameData** grPerks = grData->rank_perks;
	int grPerksNum = grData->n_rank_perks;
	for(i = 0; i < grPerksNum; i++) {
		printf("CSAV001GRD Guardian perk %d: %s\n", i, grPerks[i]->perk_data_path);
	}

// Room stuff - Decorations and the gun rack
	OakSave__CrewQuartersSaveData* roomData = charData->crew_quarters_room;
	printf("CSAV001ROM Room number: %d\n", roomData->preferred_room_assignment);
	int numDecor = roomData->n_decorations;
	OakSave__CrewQuartersDecorationSaveData** roomDecor = roomData->decorations;
	printf("CSAV001ROM Room data object: %s\n", roomData->room_data_path);
	for(i = 0; i < numDecor; i++) {
		printf("CSAV001ROM Decoration %d, index %d: %s\n", i, roomDecor[i]->decoration_index, roomDecor[i]->decoration_data_path);
	}
	OakSave__CrewQuartersGunRackSaveData* rackData = charData->crew_quarters_gun_rack;
	int numGunsOnRack = rackData->n_rack_save_data;
	OakSave__CrewQuartersGunRackItemSaveData** rackGuns = rackData->rack_save_data;
	for(i = 0; i < numGunsOnRack; i++) {
		printf("CSAV001ROM Gun %d on rack: %s, item code %s\n", i, rackGuns[i]->slot_asset_path, "TODO - PUT ITEM CODE HERE!");
	}

// ECHO Log Listing Facility/400
	OakSave__EchoLogSaveGameData** echoLogs = charData->unlocked_echo_logs;
	int numEchoLogs = charData->n_unlocked_echo_logs;
	for(i = 0; i < numEchoLogs; i++) {
		printf("CSAV001ECH ECHO Log %d: %s | played: %d\n", i, echoLogs[i]->echo_log_path, echoLogs[i]->has_been_seen_in_log);
	}

// "Game Stats Data"
	OakSave__GameStatSaveGameData** statData = charData->game_stats_data;
	int numStatData = charData->n_game_stats_data;
	for(i = 0; i < numStatData; i++) {
		printf("CSAV001STA Stat %d: %s | value %lu\n", i, statData[i]->stat_path, statData[i]->stat_path);
	}

// Active fast travel stations (the first one is for blacklisted stations, the other is the real deal), the last is the last active one
	int numActiveFTStations = charData->n_active_travel_stations;
	char** activeFTStations = charData->active_travel_stations;
	for(i = 0; i < numActiveFTStations; i++) {
		printf("CSAV001FTM Active fast travel station %d: %s\n", i, activeFTStations[i]);
	}
	OakSave__ActiveFastTravelSaveData** blFtData = charData->active_or_blacklisted_travel_stations;
	int numBlFtData = charData->n_active_or_blacklisted_travel_stations;
	for(i = 0; i < numBlFtData; i++) {
		printf("CSAV001FTM Blacklisted fast travel machine %d: %s | blacklisted: %d\n", i, blFtData[i]->active_travel_station_name, blFtData[i]->blacklisted);
	}
	OakSave__PlaythroughActiveFastTravelSaveData** ftData = charData->active_travel_stations_for_playthrough;
	
	int numFtData = charData->n_active_travel_stations_for_playthrough;
	for(i = 0; i < numFtData; i++) {
		OakSave__ActiveFastTravelSaveData** ftForPT = ftData[i]->active_travel_stations;
		int numFtForPT = ftData[i]->n_active_travel_stations;
		for(j = 0; j < numFtForPT; j++) {
			printf("CSAV001FTM FT station on playthrough %d, index %d: %s | blacklisted: %d\n", i, j, ftForPT[j]->active_travel_station_name, ftForPT[j]->blacklisted);
		}
	}
	
	char** lastStation = charData->last_active_travel_station_for_playthrough;
	int numLastStation = charData->n_last_active_travel_station_for_playthrough;
	for(i = 0; i < numLastStation; i++) {
		printf("CSAV001FTM Last active FT station for playthrough %d: %s\n", i, lastStation[i]);
	}
		

// Inventory (I know good and well you searched for that just now, if not, thanks for RTFS'ing)
	OakSave__OakInventoryItemSaveGameData** invItems = charData->inventory_items;
	int numInvItems = charData->n_inventory_items;
	for(i = 0; i < numInvItems; i++) {
		printf("CSAV001INV Backpack item %d, ", i);
		printf("Item pickup index %d, ", invItems[i]->pickup_order_index);
		printf("Serial number %x, ", invItems[i]->item_serial_number.data);
		printf("Skin %s\n", invItems[i]->weapon_skin_path);
		//printf("CSAV001INV For gun %d: Inventory data: %s\n", i, invItems[i]->development_save_data->inventory_data);
		//printf("CSAV001INV For gun %d: Balance data: %s\n", i, invItems[i]->development_save_data->inventory_balance_data);
		//printf("CSAV001INV For gun %d: Manufacturer data: %s\n", i, invItems[i]->development_save_data->manufacturer_data);
	}

// Customizations
	int numCustomizations = charData->n_selected_customizations;
	char** customizations = charData->selected_customizations;
	for(i = 0; i < numCustomizations; i++) {
		printf("CSAV001CUS Customization %d: %s\n", i, customizations[i]);
	}
	int numEmotes = charData->n_equipped_emote_customizations;
	char** emotes = charData->selected_customizations;
	for(i = 0; i < numEmotes; i++) {
		printf("CSAV001CUS Emote %d: %s\n", i, emotes[i]);
	}
	int numColors = charData->n_selected_color_customizations;
	OakSave__CustomPlayerColorSaveGameData** colorData = charData->selected_color_customizations;
	for(i = 0; i < numColors; i++) {
		printf("CSAV001CUS Color %d: Primary = (%f, %f, %f), Secondary = (%f, %f, %f), using default color: %d, using split color: %d\n", i, colorData[i]->applied_color->x, colorData[i]->applied_color->y, colorData[i]->applied_color->z, colorData[i]->split_color->x, colorData[i]->split_color->y, colorData[i]->split_color->z, colorData[i]->use_default_color, colorData[i]->use_default_split_color);
	}

// Customizations
	char** customizationsList = charData->selected_customizations;
	int custListLen = charData->n_selected_customizations;
	for(i = 0; i < custListLen; i++) {
		printf("CSAV001CST Customization %d: %s\n", i, customizationsList[i]);
	}
	
// Challenges
	OakSave__ChallengeSaveGameData** challenges = charData->challenge_data;
	int numChallenges = charData->n_challenge_data;
	for(i = 0; i < numChallenges; i++) {
		printf("CSAV001CHL Challenge %d is %s: | completed: %d, active: %d, completed progress level: %d, progress: %d\n", i, challenges[i]->challenge_class_path, challenges[i]->currently_completed, challenges[i]->is_active, challenges[i]->completed_progress_level, challenges[i]->progress_counter);
		/* COMMENTED OUT - I'M GETTING A COMPILER AND IT'S LATE AND I'M TIRED
		printf("CSAV001CHL Rewards for challenge %d:\n", i);
		OakSave__OakChallengeRewardSaveGameData rewards = challenges[i]->challenge_reward_info;
		int numRewards = challenges[i]->n_challenge_reward_info;
		for(j = 0; j < numRewards; j++) {
			printf("CSAV001CHL Reward %d for challenge %d - claimed: %d\n", j, i, rewards[j]->challenge_reward_claimed);
		}
		*/
	}


// Money (ICL)
	OakSave__InventoryCategorySaveData** icl = charData->inventory_category_list;
	int numICL = charData->n_inventory_category_list;
	for(i = 0; i < numICL; i++) {
		printf("CSAV001ICL InvCatList %d: %x | quantity: %lu\n", i, icl[i]->base_category_definition_hash, icl[i]->quantity);
	}

// Mayhem Mode
	OakSave__GameStateSaveData** sgData = charData->game_state_save_data_for_playthrough;
	int sgDataLen = charData->n_game_state_save_data_for_playthrough;
	for(i = 0; i < sgDataLen; i++) {
		printf("CSAV001MHM Mayhem level on playthrough %d: %d\n", i, sgData[i]->mayhem_level);
		printf("CSAV001MHM Mayhem seed for playthrough %d: %x\n", i, sgData[i]->mayhem_random_seed);
	}

// Free the thing
	oak_save__character__free_unpacked(charData, NULL);
	return 0;
}
	
