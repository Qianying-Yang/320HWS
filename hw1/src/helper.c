#include "helper.h"

int hasKey(const char *key) {
    if(key == NULL) {
        return 0;
    }
    else {
        return 1;
    }
}

int getRow(unsigned short mod) {
    int row = mod & 0x00F0;
    return row/16;
}

int getCol(unsigned short mod) {
    int col = mod & 0x000F;
    return col;
}

void constructTable(char *table, char *alphabet, int row, int col) {
    int i = 0;
    int j = 0;
    while(i < row) {
        j = 0;
        while(j<col) {
            char *currAlpha = alphabet + (i*col + j);
            *(table + (i*col + j)) = *currAlpha;
            j++;
        }
        i++;
    }
}

int convertToInt(int num) {
    if(num < 58 && num > 47) {
        num = num - 48;
    }
    else {
        num = num - 55;
    }
    return num;
}

int converToChar(int num) {
    if(num < 10) {
        num = num + 48;
    }
    else {
        num = num + 55;
    }
    return num;
}

void fmKey(const char *key, const char *alphabet) {
    int len = sizeof(fm_key);
    int i = 0;
    while(i < len) {
        if(i > 26) {
            *(fm_key + i) = '\0';
        }
        else {
            char c = *(alphabet + i);
            *(fm_key + i) = c;
        }
        i++;
    }
}

void putSpaceTable(char *table, int index) {
    *(table + index) = 'x';
    *(table + index + 1) = 'x';
}

void putMorseTable(char *table, int index, const char *morse, int morseLen) {
    int i = 0;
    while(i < morseLen) {
        char currMorse = *(morse + i);
        *(table + index) = currMorse;
        i++;
        index++;
    }
    *(table + index) = 'x';
}

void printMorse(const char *morse, int morseLen) {
    int i = 0;
    while(i < morseLen) {
        char currMorse = *(morse + i);
        printf("%c\n", currMorse);
        i++;
    }
}

int match(const char *key, char a, char b, char c) {
    if(*key == a && *(key + 1) == b && *(key + 2) == c)  {
        return 0;
    }
    else {
        return 100;
    }
}

void shiftTable(char *table, int index) {
    int i = 0;
    while(i < index) {
        if(i < 3) {
            *(table + i) = '\0';
        }
        else {
            *(table + (i-3)) = *(table + (i));
        }
        i++;
    }
}

void clearTable(char *table) {
    int i = 0;
    while(i < sizeof(table)) {
        *(table + i) = '\0';
        i++;
    }
}