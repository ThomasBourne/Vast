#include "save.h"


//not needed for VC but g++ and gcc require (also nick's VS?)
#include <limits.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

static int numPlaces(int n) { //returns the number of units taken up by a number
	//Fastest way to preform check, also handy is readability
	if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
	if (n < 10) return 1;
	if (n < 100) return 2;
	if (n < 1000) return 3;
	if (n < 10000) return 4;
	if (n < 100000) return 5;
	if (n < 1000000) return 6;
	if (n < 10000000) return 7;
	if (n < 100000000) return 8;
	if (n < 1000000000) return 9;
	return 0;
}

void SetDefaults(SaveState* state) { //sets each gameElement to 0
	for (iter i = 0; i < SS_GameContents_count; i++)
	{
		*(state->gameElementsStart + i) = 0;
	}
}

SaveState* SS_new(const char* path) {
	SaveState* state = (SaveState*)malloc(sizeof(SaveState)); //create a new SaveState on heap
	state->gameElementsStart = (unsigned*)malloc(sizeof(unsigned) * (unsigned)SS_GameContents_count); //allocate the space for gameElements[SS_GameContents_count]

	state->savePath = (char*)malloc(strlen(path) + 1); //allocate the length of the path + \0 to heap
	strcpy(state->savePath, path); //copy the path to savePath (as cant do = as that would loose the the malloc path

	SS_read(state); //read the contents back into the SaveState->gameElements

	return state; //return the created SaveState* to be used in SaveState* state = SS_new("C:\path");
}
SaveState* SS_blank(void) { //depreciated no comments needed
	SaveState* state = (SaveState*)malloc(sizeof(SaveState));
	state->savePath = (char*)malloc(1);

	state->gameElementsStart = (unsigned*)malloc(sizeof(unsigned) * (unsigned)SS_GameContents_count);

	strcpy(state->savePath, "");

	for (iter i = 0; i < SS_GameContents_count; i++)
	{
		*(state->gameElementsStart + i) = 0;
	}

	return state;
}
void SS_read(SaveState* state) {
	FILE* saveFile;
	if ((saveFile = fopen(state->savePath, "r")) == NULL) { //if file does not exist
		SetDefaults(state); //zero every element
		//SS_write(state); //create a file and write the zeroed elements to it
		return; //can exit now as all below is not needed as both the new file and array contents are zeroed
	}

	fseek(saveFile, 0, SEEK_END); //move to end of file contents
	long len = ftell(saveFile); //find length of file (by position of SEEK_END)
	fseek(saveFile, 0, SEEK_SET); //move back to start of file to start read

	char* buffer = (char*)malloc(len + 1); //allocate the length of file + \0 to buffer
	strcpy(buffer, ""); //null terminate

	fread(buffer, 1, len, saveFile); //read the contents of saveFile into buffer


	fclose(saveFile); //close the FILE*


	unsigned char currentContent = (unsigned char)((enum SS_GameContents)0); //the enum SS_GameContents is here to show what its intention is (current gameElement

	char currentContentNum[MAX_INT_BUFFER_LEN]; //allocate a reasonably large array for the current number (to be slapped into a string)

	unsigned char numSize = 0; //size of current string in currentContentNum

	for (iter i = 0; i < strlen(buffer); i++) //loop through all file contents
	{
		if (currentContent < SS_GameContents_count && *(buffer + i) == BREAK_KEY) { //if the currentElement is less that the count of elements and the current char in the buffer is the break key
			currentContentNum[numSize + 1] = '\0'; //null terminate the current string
			*(state->gameElementsStart + currentContent) = atoi(currentContentNum); //set the current element to (converted from str to int) the current disected part of the buffer (file)
			for (iter j = 0; j < MAX_INT_BUFFER_LEN; j++) //reset all the elements to 0 (could be [currentContentNum[0] = '\0') and is the only bit faster in c++
			{
				currentContentNum[j] = 0;
			}
			numSize = 0; //reset the current size of the str in currentContentNum
			currentContent++; //increment the current game element selected
		} else { //otherwise add the current char in the buffer to the array and increment the size
			currentContentNum[numSize] = *(buffer + i);
			numSize++;
		}
	}
	
	free(buffer);
}
	
void SS_write(SaveState* state) {
	char* buffer = (char*)malloc(MAX_STRING_LENGTH); //create a very long string [heap]
	if (buffer == NULL) { //fail check (cant hurt this dosnt need to be too quick)
		printf("Failed to allocate buffer to write save to file\n");
		return;
	}
	strcpy(buffer, ""); //null terminate immidiatly


	for (enum SS_GameContents i = 0; i < SS_GameContents_count; i++) //gather all elements and stick in a buffer seperated by the BREAK_KEY
	{
		char* currentContent = (char*)malloc(numPlaces(*(state->gameElementsStart + i)) + 2); //allocate a buffer the number of units in base 10 the current game element + 1 for null termination
		if (currentContent == NULL) { //fail check (cant hurt this dosnt need to be too quick)
			printf("Failed to allocate buffer to write save to file\n");
			free(buffer);
			return;
		}

		//iota depreciated or something so microsoft use _iota
		_itoa(*(state->gameElementsStart + i), currentContent, BASE10); //copy the current gameElement into currentContext buffer

		strcpy(buffer + strlen(buffer), (const char*)currentContent); //copy the contents of currentContent (the current game element in str) into buffer (const char* for definate conversion & speeed)

		strcpy(buffer + strlen(buffer), BREAK_KEY_STR); //add a break key to prepare for next game element

		free(currentContent); //error here but not sure why cuz only sometimes
	}

	FILE* fp; //open a file with the save path in write mode
	if ((fp = fopen(state->savePath, "w")) == NULL) {
		printf("FAILURE TO SAVE AS FILE PATH IS EITHER INVALID OR FILE WAS DESTROYED: file missing at this path: %s", state->savePath);
		free(buffer);
	}

	fprintf(fp, "%s", buffer); //write the buffer to the file

	fclose(fp); //free FILE* fp

	free(buffer); //free buffer
}

void SS_delete(SaveState* state) {
	free(state->gameElementsStart);
	free(state->savePath);
	free(state);
}