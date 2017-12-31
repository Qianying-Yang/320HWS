#include "const.h"

int hasKey(const char *key);
int getRow(unsigned short mod);
int getCol(unsigned short mod);
void constructTable(char *table, char *alphabet, int row, int col);
int convertToInt(int num);
int converToChar(int num);
void fmKey(const char *key, const char *alphabet);
void putSpaceTable(char *table, int index);
void printMorse(const char *morse, int morseLen);
void putMorseTable(char *table, int index, const char *morse, int morseLen);
int match(const char *key, char a, char b, char c);
void shiftTable(char *table, int index);
void clearTable(char *table);