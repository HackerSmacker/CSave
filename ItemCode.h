#ifndef ITEMCODE_H
#define ITEMCODE_H 1

#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"

void dumpSerial(ProtobufCBinaryData item_serial_number);
uint8_t* xorSaveData(uint8_t* data, uint8_t seed);
uint8_t* xorItemSerialData(uint8_t* data, uint32_t seed, int len);
void loadLookupTables();
char* lookupInTable(int tableIndex, int lineNumber);
int findLookupTable(char* searchText);
int findInTable(int tableIndex, char* searchText);

#endif
