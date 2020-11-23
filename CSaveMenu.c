#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>

char* currentFile;
int loopContinue = 1;
FILE* fp;
char path[8192];
char* command;
char* myLine;
char choice[32];
char input[256];
char filenameNL[2048];
char* filename;
char* filename2;
char filenameNL2[2048];
char pcInNL[2048];
char pcOutNL[2048];
char* pcIn;
char* pcOut;

void drawMainMenu() {
	printf("0    File Save Files\n");
	printf("1    Find Protobuf Files\n");
	printf("2    Convert Save to Protobuf Format\n");
	printf("3    Convert Protobuf Format to Save\n");
	printf("4    Convert Profile to Protobuf Format\n");
	printf("5    Convert Protobuf Format to Profile\n");
	printf("6    Get Save Information\n");
	printf("7    Get Profile Information\n");
	printf("8    Start Save Editor\n");
	printf("9    Start Profile Editor\n");
	printf("A    Convert Saves Between Platforms\n");
	printf("Z    Exit\n");
	printf("Enter option --> ");
}


void listDirectory(char* path, char* grep) {
	struct dirent* de;
	DIR* dr = opendir(path);
	if(dr == NULL) {
		printf("Failed to list files.\n");
		return;
	}
	printf("File Listing\n");
	while((de = readdir(dr)) != NULL) {
		printf("%s\n", de->d_name);
	}
}

void saveConvert() {
	printf("Convert Save\n");
	printf("Please specify the input save file.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename = malloc(strlen(filenameNL) - 1);
	strncpy(filename, filenameNL, (strlen(filenameNL) - 1));
	filename[strlen(filenameNL) - 1] = '\0';

	printf("Please specify the output save file.\n");
	printf("Enter file --> ");
	fgets(filenameNL2, 1024, stdin);
	filename2 = malloc(strlen(filenameNL2) - 1);
	strncpy(filename2, filenameNL2, (strlen(filenameNL2) - 1));
	filename2[strlen(filenameNL2) - 1] = '\0';

	printf("Please specify the input platform code (see manual).\n");
	printf("Enter value --> ");
	fgets(pcInNL, 1024, stdin);
	pcIn = malloc(strlen(pcInNL) - 1);
	strncpy(pcIn, pcInNL, (strlen(pcInNL) - 1));
	pcIn[strlen(pcInNL) - 1] = '\0';

	printf("Please specify the output platform code (see manual).\n");
	printf("Enter value --> ");
	fgets(pcOutNL, 1024, stdin);
	pcOut = malloc(strlen(pcOutNL) - 1);
	strncpy(pcOut, pcOutNL, (strlen(pcOutNL) - 1));
	pcOut[strlen(pcOutNL) - 1] = '\0';

	printf("Will convert %s to %s\n", filename, filename2);
	command = malloc(strlen("SaveConvert ") + strlen(filename) + 1 + strlen(filename2) + 8);
	strcpy(command, "SaveConvert ");
	strcat(command, filename);
	strcat(command, " ");
	strcat(command, filename2);
	strcat(command, " ");
	strcat(command, pcIn);
	strcat(command, " ");
	strcat(command, pcOut);
	fp = popen(command, "r");
	if(fp == NULL) {
		printf("Failed to execute SaveConvert!\n");
		return;
	}
	while(fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", myLine);
	}
	printf("Execution complete\n");
}

void saveToProto() {
	printf("Save to Protobuf\n");
	printf("Please specify a filename.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename = malloc(strlen(filenameNL) - 1);
	strncpy(filename, filenameNL, (strlen(filename) - 1));
	filename[strlen(filenameNL) - 1] = '\0';
	printf("Will convert %s\n", filename);
	command = malloc(strlen("SaveToProto ") + strlen(filenameNL) + 1);
	strcpy(command, "SaveToProto ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		printf("Failed to execute SaveToProto!\n");
		return;
	}
	while(fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", myLine);
	}
	printf("Execution complete\n");
}

void saveToProtoProfile() {
	printf("Profile to Protobuf\n");
	printf("Please specify a filename.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename = malloc(strlen(filenameNL) - 1);
	strncpy(filename, filenameNL, (strlen(filenameNL) - 1));
	filename[strlen(filenameNL) - 1] = '\0';
	printf("Will convert %s\n", filename);
	command = malloc(strlen("ProfileToProto ") + strlen(filename) + 1);
	strcpy(command, "ProfileToProto ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		printf("Failed to execute ProfileToProto!\n");
		return;
	}
	while(fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", myLine);
	}
	printf("Execution complete\n");

}

void protoToSave() {
	printf("Protobuf to Save\n");
	printf("Please specify a protobuf file.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename = malloc(strlen(filenameNL) - 1);
	strncpy(filename, filenameNL, (strlen(filenameNL) - 1));
	filename[strlen(filenameNL) - 1] = '\0';

	printf("Please specify the original file.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename2 = malloc(strlen(filenameNL) - 1);
	strncpy(filename2, filenameNL, (strlen(filenameNL) - 1));
	filename2[strlen(filenameNL) - 1] = '\0';

	printf("Will convert %s\n", filename);
	command = malloc(strlen("ProtoToSave ") + strlen(filename) + 1 + strlen(filename2));
	strcpy(command, "ProtoToSave ");
	strcat(command, filename);
	strcat(command, " ");
	strcat(command, filename2);
	fp = popen(command, "r");
	if(fp == NULL) {
		printf("Failed to execute ProtoToSave!\n");
		return;
	}
	while(fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", myLine);
	}
	printf("Execution complete\n");
}

void protoToSaveProfile() {
	printf("Protobuf to Profile\n");
	printf("Please specify a protobuf file.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename = malloc(strlen(filenameNL) - 1);
	strncpy(filename, filenameNL, (strlen(filename) - 1));
	filename[strlen(filenameNL) - 1] = '\0';

	printf("Please specify the original file.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename2 = malloc(strlen(filenameNL) - 1);
	strncpy(filename2, filenameNL, (strlen(filenameNL) - 1));
	filename2[strlen(filenameNL) - 1] = '\0';

	printf("Will convert %s\n", filename);
	command = malloc(strlen("ProtoToProfile ") + strlen(filename) + 1 + strlen(filename2));
	strcpy(command, "ProtoToProfile ");
	strcat(command, filename);
	strcat(command, " ");
	strcat(command, filename2);
	fp = popen(command, "r");
	if(fp == NULL) {
		printf("Failed to execute ProtoToProfile!\n");
		return;
	}
	while(fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", myLine);
	}
	printf("Execution complete\n");
}

void saveUnpack() {
	printf("View Save Information\n");
	printf("Please specify protobuf file.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename = malloc(strlen(filenameNL) - 1);
	strncpy(filename, filenameNL, (strlen(filename) - 1));
	filename[strlen(filenameNL) - 1] = '\0';
	printf("Will process %s\n", filename);
	command = malloc(strlen("SaveUnpack ") + strlen(filename) + 1);
	strcpy(command, "SaveUnpack ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		printf("Failed to execute SaveUnpack\n");
		return;
	}
	while(fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", myLine);
	}
	printf("Execution complete\n");
}

void profUnpack() {
	printf("View Profile Information\n");
	printf("Please specify protobuf file.\n");
	printf("Enter file --> ");
	fgets(filenameNL, 1024, stdin);
	filename = malloc(strlen(filenameNL) - 1);
	strncpy(filename, filenameNL, (strlen(filename) - 1));
	filename[strlen(filenameNL) - 1] = '\0';
	printf("Will process %s\n", filename);
	command = malloc(strlen("ProfileUnpack ") + strlen(filename) + 1);
	strcpy(command, "ProfileUnpack ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		printf("Failed to execute ProfileUnpack!\n");
		return;
	}
	while(fgets(path, sizeof(path), fp) != NULL) {
		printf("%s", myLine);
	}
	printf("Execution complete\n");
}

int main(int argc, char** argv) {
	// Start.
	do {
		drawMainMenu();
		fgets(input, 256, stdin);
		if(strcmp(input, "z\n") == 0) {
			loopContinue = 0;
		}
		else if(strcmp(input, "0\n") == 0) {
			listDirectory(".", ".sav");
		}
		else if(strcmp(input, "1\n") == 0) {
			listDirectory(".", ".proto");
		}
		else if(strcmp(input, "2\n") == 0) {
			saveToProto();
		}
		else if(strcmp(input, "3\n") == 0) {
			protoToSave();
		}
		else if(strcmp(input, "4\n") == 0) {
			saveToProtoProfile();
		}
		else if(strcmp(input, "5\n") == 0) {
			protoToSaveProfile();
		}
		else if(strcmp(input, "6\n") == 0) {
			saveUnpack();
		}
		else if(strcmp(input, "7\n") == 0) {
			profUnpack();
		}
	} while(loopContinue == 1);

	// Exit.
	printf("Thank you for choosing CSave.\n");
}
