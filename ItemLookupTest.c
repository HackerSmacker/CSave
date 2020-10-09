#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "ItemCode.h"
#include "OakShared.pb-c.h"
#include "OakSave.pb-c.h"

int main() {
	printf("CSAV001ILT Item Lookup Test Program\n");
	loadLookupTables();
	return 0;
}
