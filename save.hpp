#ifndef SAVE_HPP
#define SAVE_HPP

#include "window.hpp"

namespace save {
	extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

		typedef unsigned char BYTE;
		typedef unsigned int uint;

		const char* breakKey = ";";

		typedef struct SaveStruct {
			const char* savePath;
			//to do with the game
			uint currentLevel;

			//to do with the player
			uint maxHealth;
			uint health;
			uint Dourn;

			//when adding new features add them to:
			/*
			* NewSaveState()
			* ReadSave()
			* WriteSave()
			*/

			//AMOUNT OF THINGS TO SERPERATE WHEN WRITING TO A FILE
			BYTE contents = 4; //CHANGE ME ON UPDATE
		} SaveState;

		SaveState* EmptyState(const char*);
		SaveState* NewState(uint, uint, uint, uint, const char*);
		void		WriteSave(SaveState*);
		void		ReadSave(SaveState*);
		SaveState* ReadSaveFromPath(const char*);
		void		CreateSaveFile(const char*);
		void		PrintSave(SaveState*);
		bool		FileExists(const char*);


		SaveState* EmptyState(const char* path = "") {
			SaveState obj;
			obj.savePath = path;
			return &obj;
		}

		SaveState* NewState(uint currentLevel, uint maxHealth, uint health, uint Dourn, const char* path = "") {
			SaveState obj;
			obj.savePath = path;
			obj.currentLevel = currentLevel;
			obj.maxHealth = maxHealth;
			obj.health = health;
			obj.Dourn = Dourn;
			//WriteSave(&obj); //could call but makes a bit more messy to use when not wanted
			return &obj;
		}

		void WriteSave(SaveState* saveState) {
			if (saveState->savePath == "")
				return;
			if (!FileExists(saveState->savePath))
				CreateSaveFile(saveState->savePath);
			FILE* handle;

			char contents[] = "";
			char convCap[255]; //stores the converted value of the int to string casts

			sprintf(convCap, "%d", saveState->currentLevel); //this just likes to break
			strcat(contents, convCap);
			strcat(contents, breakKey);

			sprintf(convCap, "%d", saveState->maxHealth);
			strcat(contents, convCap);
			strcat(contents, breakKey);

			sprintf(convCap, "%d", saveState->health);
			strcat(contents, convCap);
			strcat(contents, breakKey);

			sprintf(convCap, "%d", saveState->Dourn);
			strcat(contents, convCap);
			strcat(contents, breakKey);

			if ((handle = fopen(saveState->savePath, "w")) == ((void*)0))
			{
				CreateSaveFile(saveState->savePath);
				handle = fopen(saveState->savePath, "w");
			}


			fprintf(handle, "%s", contents);

			fclose(handle);
		}

		void ReadSave(SaveState* saveState) {
			if (saveState->savePath == "")
				return;
			FILE* file;
			// Check if there was an error.
			if ((file = fopen(saveState->savePath, "r")) == ((void*)0)) {
				CreateSaveFile(saveState->savePath);
				file = fopen(saveState->savePath, "r");
			}
			// Get the file length
			fseek(file, 0, SEEK_END);
			long length = ftell(file);
			fseek(file, 0, SEEK_SET);
			// Create the string for the file contents.
			char* buffer = (char*)malloc(sizeof(char) * (length + 1));
			buffer[length] = '\0';

			fread(buffer, sizeof(char), length, file);

			fclose(file);

			//catch data
			//here the contents need to be disected

			BYTE count{ 0 };
			char saveBuffer[255]{ '\0' };
			for (uint i = 0; i < length; i++)
			{
				if (buffer[i] == breakKey[0]) {
					switch (count)
					{
					case(0):
					{
						saveState->currentLevel = atoi(saveBuffer);
						break;
					}
					case(1):
					{
						saveState->maxHealth = atoi(saveBuffer);
						break;
					}
					case(2):
					{
						saveState->health = atoi(saveBuffer);
						break;
					}
					case(3):
					{
						saveState->Dourn = atoi(saveBuffer);
						break;
					}
					default:
						break;
					}
					*saveBuffer = { '\0' };
					count++;
				}
				else {
					if (saveBuffer != "\0")
						strncat(saveBuffer, &buffer[i], 1);
					else {
						saveBuffer[0] = buffer[i];
					}
				}
			}
			//free the allocated string space.
			free(buffer);
		}

		SaveState* ReadSaveFromPath(const char* path) {
			SaveState state;
			state.savePath = path;
			ReadSave(&state);
			return &state;
		}

		void CreateSaveFile(const char* path) {
			FILE* handle;
			handle = fopen(path, "a");
			fclose(handle);
		}

		void PrintSave(SaveState* saveState) {
			printf("%d", saveState->currentLevel);
			printf("\n");

			printf("%d", saveState->maxHealth);
			printf("\n");

			printf("%d", saveState->health);
			printf("\n");

			printf("%d", saveState->Dourn);
			printf("\n");
		}

		bool FileExists(const char* path) {
			FILE* handle;
			handle = fopen(path, "r");
			if (handle == ((void*)0)) {
				return false;
			}
			fclose(handle);
			return true;
		}

		void ChangeDefaultSave(SaveState* newState, SaveState& currentSave) {
			currentSave = *(newState);
		}
	};
}
#endif