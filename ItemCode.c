#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"
#include "OakProfile.pb-c.h"
#include "ItemCode.h"

// File descriptors for lookup tables
FILE** lookupTableFiles;

// Array of strings containing all that good stuff
char** lookupTable;

// Number of LT files:
int lookupTableCount;

// Filenames to lookup tables
const char lookupTableFilenames[50][256] = {
	"LookupTables/BPInvPart_AR_COV_C.txt",
	"LookupTables/BPInvPart_AR_DAL_C.txt",
	"LookupTables/BPInvPart_AR_TOR_C.txt",
	"LookupTables/BPInvPart_Artifact_C.txt",
	"LookupTables/BPInvPart_ATL_AR_C.txt",
	"LookupTables/BPInvPart_ATL_HW_C.txt",
	"LookupTables/BPInvPart_ClassMod_C.txt",
	"LookupTables/BPInvPart_Customization_Head_C.txt",
	"LookupTables/BPInvPart_Customization_Skin_C.txt",
	"LookupTables/BPInvPart_Dahl_SMG_C.txt",
	"LookupTables/BPInvPart_GrenadeMod_C.txt",
	"LookupTables/BPInvPart_HW_COV_C.txt",
	"LookupTables/BPInvPart_HW_TOR_C.txt",
	"LookupTables/BPInvPart_HW_VLA_C.txt",
	"LookupTables/BPInvPart_Hyperion_Shotgun_C.txt",
	"LookupTables/BPInvPart_JAK_AR_C.txt",
	"LookupTables/BPInvPart_Jakobs_Pistol_C.txt",
	"LookupTables/BPInvPart_MAL_SR_C.txt",
	"LookupTables/BPInvPart_Maliwan_SMG_C.txt",
	"LookupTables/BPInvPart_PS_ATL_C.txt",
	"LookupTables/BPInvPart_PS_COV_C.txt",
	"LookupTables/BPInvPart_PS_DAL_C.txt",
	"LookupTables/BPInvPart_PS_MAL_C.txt",
	"LookupTables/BPInvPart_PS_TOR_C.txt",
	"LookupTables/BPInvPart_PS_VLA_C.txt",
	"LookupTables/BPInvPart_SG_JAK_C.txt",
	"LookupTables/BPInvPart_SG_MAL_C.txt",
	"LookupTables/BPInvPart_SG_TED_C.txt",
	"LookupTables/BPInvPart_SG_Torgue_C.txt",
	"LookupTables/BPInvPart_Shield_C.txt",
	"LookupTables/BPInvPart_SM_Hyperion_C.txt",
	"LookupTables/BPInvPart_SM_TED_C.txt",
	"LookupTables/BPInvPart_SR_DAL_C.txt",
	"LookupTables/BPInvPart_SR_HYP_C.txt",
	"LookupTables/BPInvPart_SR_JAK_C.txt",
	"LookupTables/BPInvPart_Tediore_Pistol_C.txt",
	"LookupTables/BPInvPart_VLA_AR_C.txt",
	"LookupTables/BPInvPart_VLA_SR_C.txt",
	"LookupTables/BPInvPartData_EridianFabricator_C.txt",
	"LookupTables/BPVehiclePart_C.txt",
	"LookupTables/DownloadableEntitlementPartData.txt",
	"LookupTables/InventoryBalanceData.txt",
	"LookupTables/InventoryCustomizationPartData.txt",
	"LookupTables/InventoryData.txt",
	"LookupTables/InventoryGenericPartData.txt",
	"LookupTables/ItemPoolData.txt",
	"LookupTables/ManufacturerData.txt",
	"LookupTables/OakDownloadableContentLicenseData.txt"
};


// de-XOR the save file.
uint8_t* xorSaveData(uint8_t* data, uint8_t seed) {
	int dataLen = strlen(data);
	uint8_t* out;
	out = malloc(dataLen);
	if(seed == 0) {
		strcpy(out, data);
	}
	int i;
	uint8_t currentByte = 0;
	for(i = 0; i < dataLen; i++) {
		currentByte = (seed >> 5) & 0xFFFFFFFF;
		currentByte = ((currentByte * 0x10A860C1) % 0xFFFFFFFB);
		out[i] = ((data[i] ^ currentByte) & 0xFF);
	}
	return out;
}


// de-XOR an item serial.
uint8_t* xorItemSerialData(uint8_t* data, uint32_t seed, int len) {
	uint32_t xorMaskItself = (seed >> 5) & 0xFFFFFFFF;
	uint8_t* temp = malloc(len);
	if(seed == 0) {
		return data;
	}
	int i;
	for(i = 0; i < len; i++) {
		xorMaskItself = (xorMaskItself * 0x10A860C1) % 0xFFFFFFFB;
		temp[i] = ((data[i] ^ xorMaskItself) & 0xFF);
	}
	return temp;
}


// Print out info about a serial to the console.
void dumpSerial(ProtobufCBinaryData item_serial_number) {
	// The actual data and the length.
	uint8_t* data = item_serial_number.data;
	int len = item_serial_number.len;
	int i;
	// Print out the data, char by char.
	printf("CSAV001INV Parsing serial ");
	for(i = 0; i < len; i++) {
		printf("%x", data[i]);
	}
	printf("\n");
	// Done printing; get the magic number and make sure it's 1 - removed because this is dumb
	uint8_t magicNumber = data[0];
	// Do some shifts to obtain the XOR seed. Keep in mind this code won't work on big-endian machines (so no SGI machines).
	uint32_t xorSeed = data[1] | (data[2] << 8) | (data[3] << 8) | (data[4] << 8);
	printf("CSAV001INV XOR seed for this item: %x\n", xorSeed);
	// Take off 5 bytes for the magic number and seed, the rest is the XOR'd data.
	int xorDataLen = len - 5;
	uint8_t* xorData = malloc(len - 5);
	// Copy the rest of the serial to the buffer so it can be de-XOR'd.
	memcpy(xorData, data + 5, xorDataLen);
	// Call the un-XOR function.
	uint8_t* decryptedData = xorItemSerialData(xorData, xorSeed, xorDataLen);
	// Print out the encrypted and decrypted data
	printf("CSAV001INV Payload length: %d\n", xorDataLen);
	printf("CSAV001INV Data encrypted: ");
	for(i = 0; i < xorDataLen; i++) {
		printf("%x", xorData[i]);
	}
	printf("\n");
	printf("CSAV001INV Data decrypted: ");
	for(i = 0; i < xorDataLen; i++) {
		printf("%x", decryptedData[i]);
	}
	printf("\n");

}

// Convert the InventoryBalanceData from the serial into the full path.

uint8_t* serialIBDToPath(uint8_t* ibd) {
	return NULL;
}

// Load the lookup tables into memory.
void loadLookupTables() {
	// Define variables
	int i, j, k;
	char* current;
	// Hardcoded limit on how many data files there are
	lookupTableCount = 48;
	// Allocate space for some FILEs
	lookupTableFiles = malloc(sizeof(FILE*) * lookupTableCount);
	// Print messages
	printf("CSAV001FIL Loading lookup tables... will load %d files.\n", lookupTableCount);
	// Iterate through each file...
	for(i = 1; i < lookupTableCount; i++) {
		// ...print out a message to the user...
		printf("CSAV001FIL Loading file %d: %s\n", i, lookupTableFilenames[i]);
		// ...open the file...
		lookupTableFiles[i] = fopen(lookupTableFilenames[i], "r");
		// ...make sure it actually did.
		if(lookupTableFiles[i] == NULL) {
			fprintf(stderr, "CSAV001FIL FATAL PROCESSING ERROR WHEN OPENING FILE %s\n. EXECUTION MAY NOT CONTINUE.\n", lookupTableFilenames[i]);
		}
		// Now, read the file:
		char line[2048];
		// j is the line indicator
		j = 0;
		while (fgets(line, sizeof(line), lookupTableFiles[i])) {
			strtok(line, "\n");
			printf("%s\n", line);
			j++;
		}
    	}
}
