#ifndef SAVE_HPP
#define SAVE_HPP

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                  Save                                                        ///
///                                                  ----                                                        ///
///                                                                                                              ///
/// This is done in C as std::ostream and istream are slower than FILE* plus would it be a complete project      ///
/// without C? The file contains a struct called [SaveState] and functions that manipulate it start with SS_     ///
/// the way to access the parts of the save for the file is by using the enum and using the array in the struct  ///
///                                                                                                              ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_STRING_LENGTH (sizeof(unsigned short))
#define MAX_INT_BUFFER_LEN 15
#define BREAK_KEY ';'
#define BREAK_KEY_STR ";"
#define BASE10 10

typedef unsigned char	iter;
typedef iter			siter;

enum SS_GameContents {
	SS_GameContents_CurrentLevel = 0,
	SS_GameContents_Heath = 1,
	SS_GameContents_MaxHealth = 2,
	SS_GameContents_Dourn = 3,

	SS_GameContents_count = 4 // keep at end please
};

typedef struct SaveState {
	char* savePath;

	//contents:
	//CurrentLevel
	//Heath
	//MaxHealth
	//Dourn (currency)

	unsigned* gameElementsStart;

}SaveState;


SaveState* SS_new(const char* path); //creates a new SaveState on heap, reading contents from path and passing it in
//blank was depreciated to the static function SetDefaults
SaveState* SS_blank(void); //creates a new SaveState on heap and zeros everything
void SS_read(SaveState* state); //read contents from SaveState path and store in SaveState
void SS_write(SaveState* state); //write SaveState into the SaveStates file
void SS_delete(SaveState* state); //deallocates memory

#endif