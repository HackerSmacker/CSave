#ifndef ITEMCODE_H
#define ITEMCODE_H 1

#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"

struct Item_aspect {
	int bits;
	int index;
	char* val;
};

enum Item_flags {
	flag_seen = 1,
	flag_fav = 2,
	flag_trash = 4
};

struct Item {
	/* --- These fields are commented out for now...
	int writable;
	int version;
	char* parts_category;
	char* name;
	char* object_short_name;
	struct Item_aspect balance;
	struct Item_aspect data;
	struct Item_aspect manufacturer;
	int level;
	int numberOfParts;
	struct aspect* parts;
	struct aspect* genericParts;
	int seed;
	unsigned char* wear;
	int numCustom;
	char* remainingBits;
	--- end disablement block */
	int version;
	int balance_bits;
	int balance_idx;
	char* balance;
	char* balance_short;
	char* eng_name;
	int invdata_bits;
	int invdata_idx;
	char* invdata;
	int manufacturer_bits;
	int manufacturer_idx;
	char* manufacturer;
	int level;
	char* remaining_data;
	int* part_invkeys;
	int* part_bits;
	char** parts;
	int* generic_bits;
	int* generic_parts;
};

void dumpSerial(ProtobufCBinaryData item_serial_number);
unsigned char* xorSaveData(unsigned char* data, unsigned char seed);
unsigned char* xorItemSerialData(unsigned char* data, unsigned int seed, int len);
void loadLookupTables();
char* lookupInTable(int tableIndex, int lineNumber);
int findLookupTable(char* searchText);
int findInTable(int tableIndex, char* searchText);

#endif
