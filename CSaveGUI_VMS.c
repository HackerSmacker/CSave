// FOR WIN32:
// Compile and link against PDCurses. 
// FOR UNIX:
// Compile and link against libncurses.
// FOR VMS:
// Only compiles on Alpha VMS and not VAX VMS. Good luck. The VMSCURSES image should be loaded by default.

#ifndef _WIN32
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#else
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winuser.h>
#include <winreg.h>
#endif

// Visual C++ is so incompentent I have to define these variables here.
char* menuHeaderText = "CSave Functions Menu";
int row;
int col;
char* currentFile;
int loopContinue = 1;
char filename[1024];
FILE* fp;
char path[8192];
char* command;
int outRow = 4;
int outCol = 2;
char* myLine;
char choice;


void drawMainMenu() {
	clear();
	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	mvprintw(0, (col - strlen(menuHeaderText)) / 2, "%s", menuHeaderText);
	attroff(A_BOLD);
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(2, 2, "0    File Save Files");
	mvprintw(3, 2, "1    Find Protobuf Files");
	mvprintw(4, 2, "2    Convert Save to Protobuf Format");
	mvprintw(5, 2, "3    Convert Protobuf Format to Save");
	mvprintw(6, 2, "4    Get Save Information");
	mvprintw(7, 2, "5    Get Profile Information");
	mvprintw(8, 2, "6    Start Save Editor");
	mvprintw(9, 2, "7    Start Profile Editor");
	mvprintw(10, 2, "Z    Exit");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));
	mvprintw((row - 2), 2, "Enter option --> ");
	attroff(COLOR_PAIR(3));
	refresh();
}

void defineColors() {
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
}

#ifndef _WIN32
void listDirectory(char* path, char* grep) {
	struct dirent* de;
	DIR* dr = opendir(path);
	if(dr == NULL) {
		clear();
		attron(COLOR_PAIR(4));
		mvprintw(0, 0, "Failed to open directory!");
		refresh();
		getch();
		attroff(COLOR_PAIR(4));
		clear();
		refresh();
		return;
	}
	int line = 2;
	clear();
	attron(COLOR_PAIR(1));
	mvprintw(0, (col - strlen("File Listing")) / 2, "File Listing");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	while((de = readdir(dr)) != NULL) {
		move(line, 2);
		if(strstr(de->d_name, grep) != NULL) {
			printw("%s\n", de->d_name);
			line++;
		}
	}
	attroff(COLOR_PAIR(2));
	refresh();
	getch();
}
#else
void listDirectory(char* path, char* grep) {
	return;
}
#endif

void saveToProto() {
	clear();
	attron(COLOR_PAIR(1));
	mvprintw(0, (col - strlen("Save to Protobuf")) / 2, "Save To Protobuf");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(1, 0, "Please specify a filename.");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));
	refresh();
	mvprintw((row - 2), 2, "Enter file --> ");
	attroff(COLOR_PAIR(3));
	getstr(filename);
	attron(COLOR_PAIR(2));
	mvprintw(2, 0, "Will convert %s", filename);
	refresh();
	command = malloc(strlen("SaveToProto ") + strlen(filename) + 1);
	strcpy(command, "SaveToProto ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		attron(COLOR_PAIR(4));
		mvprintw(row - 1, 0, "Failed to execute SaveToProto!");
		attroff(COLOR_PAIR(4));
		refresh();
		getch();
		return;
	}
	outRow = 4;
	outCol = 2;
	attron(COLOR_PAIR(3));
	move(outRow, outCol);
	while(fgets(path, sizeof(path), fp) != NULL) {
		myLine = malloc(col - 4);
		memcpy(myLine, path, col - 4);
		if(outRow > row - 4) {
			getch();
			clear();
			outRow = 4;
		}
		mvprintw(outRow, outCol, "%s", myLine);
		outRow++;
	}
	pclose(fp);
	getch();
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));
	mvprintw(row - 1, 0, "Execution complete");
	attroff(COLOR_PAIR(2));
	getch();
}

void protoToSave() {
	clear();
	attron(COLOR_PAIR(1));
	mvprintw(0, (col - strlen("Protobuf To Save")) / 2, "Protobuf To Save");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(1, 0, "Please specify a filename.");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));
	refresh();
	mvprintw((row - 2), 2, "Enter file --> ");
	attroff(COLOR_PAIR(3));
	getstr(filename);
	attron(COLOR_PAIR(2));
	mvprintw(2, 0, "Will convert %s", filename);
	refresh();
	command = malloc(strlen("ProtoToSave ") + strlen(filename) + 1);
	strcpy(command, "ProtoToSave ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		attron(COLOR_PAIR(4));
		mvprintw(row - 1, 0, "Failed to execute ProtoToSave");
		attroff(COLOR_PAIR(4));
		refresh();
		getch();
		return;
	}
	outRow = 4;
	outCol = 2;
	attron(COLOR_PAIR(3));
	move(outRow, outCol);
	while(fgets(path, sizeof(path), fp) != NULL) {
		myLine = malloc(col - 4);
		memcpy(myLine, path, col - 4);
		if(outRow > row - 4) {
			getch();
			clear();
			outRow = 4;
		}
		mvprintw(outRow, outCol, "%s", myLine);
		outRow++;
	}
	pclose(fp);
	getch();
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));
	mvprintw(row - 1, 0, "Execution complete");
	attroff(COLOR_PAIR(2));
	getch();
}

void saveUnpack() {
	clear();
	attron(COLOR_PAIR(1));
	mvprintw(0, (col - strlen("Save Info")) / 2, "Save Info");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(1, 0, "Please specify a filename.");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));
	refresh();
	mvprintw((row - 2), 2, "Enter file --> ");
	attroff(COLOR_PAIR(3));
	getstr(filename);
	attron(COLOR_PAIR(2));
	mvprintw(2, 0, "Will process %s", filename);
	refresh();
	command = malloc(strlen("SaveUnpack ") + strlen(filename) + 1);
	strcpy(command, "SaveUnpack ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		attron(COLOR_PAIR(4));
		mvprintw(row - 1, 0, "Failed to execute SaveUnpack!");
		attroff(COLOR_PAIR(4));
		refresh();
		getch();
		return;
	}
	outRow = 4;
	outCol = 2;
	attron(COLOR_PAIR(3));
	move(outRow, outCol);
	while(fgets(path, sizeof(path), fp) != NULL) {
		myLine = malloc(col - 4);
		memcpy(myLine, path, col - 4);
		if(outRow > row - 4) {
			getch();
			clear();
			outRow = 4;
		}
		mvprintw(outRow, outCol, "%s", myLine);
		outRow++;
	}
	pclose(fp);
	getch();
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));
	mvprintw(row - 1, 0, "Execution complete");
	attroff(COLOR_PAIR(2));
	getch();
}

void profUnpack() {
	clear();
	attron(COLOR_PAIR(1));
	mvprintw(0, (col - strlen("Profile Info")) / 2, "Profile Info");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	mvprintw(1, 0, "Please specify a filename.");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));
	refresh();
	mvprintw((row - 2), 2, "Enter file --> ");
	attroff(COLOR_PAIR(3));
	getstr(filename);
	attron(COLOR_PAIR(2));
	mvprintw(2, 0, "Will process %s", filename);
	refresh();
	command = malloc(strlen("ProfileUnpack ") + strlen(filename) + 1);
	strcpy(command, "ProfileUnpack ");
	strcat(command, filename);
	fp = popen(command, "r");
	if(fp == NULL) {
		attron(COLOR_PAIR(4));
		mvprintw(row - 1, 0, "Failed to execute ProfileUnpack!");
		attroff(COLOR_PAIR(4));
		refresh();
		getch();
		return;
	}
	outRow = 4;
	outCol = 2;
	attron(COLOR_PAIR(3));
	move(outRow, outCol);
	while(fgets(path, sizeof(path), fp) != NULL) {
		myLine = malloc(col - 4);
		memcpy(myLine, path, col - 4);
		if(outRow > row - 4) {
			getch();
			clear();
			outRow = 4;
		}
		mvprintw(outRow, outCol, "%s", myLine);
		outRow++;
	}
	pclose(fp);
	getch();
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));
	mvprintw(row - 1, 0, "Execution complete");
	attroff(COLOR_PAIR(2));
	getch();
}

int main(int argc, char** argv) {
	initscr();
	start_color();
	defineColors();
	getmaxyx(stdscr, row, col);
	// Start.
	do {
		drawMainMenu();
		choice = getch();
		if(choice == 'z') {
			loopContinue = 0;
		}
		else if(choice == '0') {
			listDirectory(".", ".sav");
		}
		else if(choice == '1') {
			listDirectory(".", ".proto");
		}
		else if(choice == '2') {
			saveToProto();
		}
		else if(choice == '3') {
			protoToSave();
		}
		else if(choice == '4') {
			saveUnpack();
		}
		else if(choice == '5') {
			profUnpack();
		}
		else {
			attron(COLOR_PAIR(4));
			mvprintw(row - 1, 0, "Please enter one of the options listed above.");
			getch();
			attroff(COLOR_PAIR(4));
			refresh();
		}
	} while(loopContinue == 1);

	// Exit.
	clear();
	mvprintw(0, 0, "Strike any key to continue");
	refresh();
	getch();
	endwin();
}