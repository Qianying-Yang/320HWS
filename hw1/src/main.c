#include <stdlib.h>

#include "hw1.h"
#include "helper.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

int main(int argc, char **argv)
{
    unsigned short mode;
    mode = validargs(argc, argv);
    int row = getRow(mode);
    int col = getCol(mode);

    debug("Mode: 0x%X", mode);

    if(mode & 0x0000) {
        USAGE(*argv, EXIT_FAILURE);
    }
    else if(mode & 0x8000){
        USAGE(*argv, EXIT_SUCCESS);
    }

    else if(mode & 0x00FF) {
        /*put the key into the table*/
        if(hasKey(key) == 1) {
            int i = 0;
            while(i < getfmLen(key)) {
                *(polybius_table + i) = *(key + i);
                i++;
            }
            // put rest of the alphabet into table.
            int lastPos = i;
            char *newStart = polybius_table + lastPos;
            i = 0;
            int j = 0;
            int m = 0;
            int index = 0;
            int found = 0;
            //printf("%i\n", sizeof(polybius_table));
            while(i<sizeof(polybius_table)) {
                if(i > getLength(polybius_alphabet)) {
                    *(newStart + m) = '\0';
                }
                else {
                    char *currAlpha = polybius_alphabet + index;
                    //printf("%c\n",*currAlpha);
                    j = 0;
                    found = 0;
                    while(j < lastPos) {
                        char currLetter = *(key + j);
                        if(*currAlpha == currLetter) {
                            found = 1;
                            j = lastPos;
                        }
                        j++;
                    }
                    if(found != 0) {
                        index++;
                    }
                    else {
                        //printf("%s\n", "trying to store");
                        *(newStart + m) = *(polybius_alphabet + index);
                        index++;
                        m++;
                    }
                }
                i++;
            }

        }

        else {
            constructTable(polybius_table, polybius_alphabet, row, col);
        }

        if((mode & 0x2000) == 0x0000) {
            /*Starts Encryption*/
            int i = 0;
            int r = 0;
            int c = 0;
            int found = 0;
            int input = getchar();
            while(input != EOF) {
                if((input == 32 || input == 10 || input == 9)) {
                    putchar(input);
                }
                else {
                    i = 0;
                    char character = (char)input;
                    while(i < sizeof(polybius_table)) {
                        char curr = *(polybius_table + i);
                        if(character == curr) {
                            r = i/col;
                            c = i%col;
                            r = converToChar(r);
                            c = converToChar(c);
                            putchar(r);
                            putchar(c);
                            found++;
                            break;
                        }
                        i++;
                    }
                    if(found == 0) {
                        USAGE(*argv, EXIT_FAILURE);
                    }
                }
                input = getchar();
            }
        }
        else {
            /*Starts decryption*/
            int num = 0;
            int input = getchar();
            while(input != EOF) {
                if((input == 32 || input == 10 || input == 9)) {
                    putchar(input);
                    input = getchar();
                }
                else {
                    num = convertToInt(input);
                    int pos = num*col;
                    input = getchar();
                    num = convertToInt(input);
                    char output = *(polybius_table + (pos+num));
                    putchar(output);
                    input = getchar();
                }
            }
        }
    }
    /*Fractionated Morse Cipher*/
    else {
        if(hasKey(key) == 0) {
            fmKey(key, fm_alphabet);
        }
        else {
            //fill in key
            int i = 0;
            while(i < getfmLen(key)) {
                *(fm_key + i) = *(key + i);
                i++;
            }
            int j = 0;
            int alphabet = 65;
            while(i < sizeof(fm_key)) {
                j = 0;
                while(j < getfmLen(key)) {
                    char current = *(key + j);
                    if(current == alphabet) {
                        alphabet++;
                        j = 0;
                    }
                    else {
                        j++;
                    }
                }
                *(fm_key + i) = alphabet;
                alphabet++;
                i++;
            }

        }

        /*starts encrypting*/
        if((mode & 0x2000) == 0x0000) {
            int input = fgetc(stdin);
            int isSpace = 0;
            int index = 0;
            while(input != EOF) {
                if(input == '\0') {
                    //clearTable(polybius_table);
                    index = 0;
                }
                if(input == 32 && isSpace == 0) {
                    isSpace++;
                    input = fgetc(stdin);
                    putSpaceTable(polybius_table, index);
                    index = index + 2;
                }
                else if(input == 32 && isSpace != 0) {
                    input = fgetc(stdin);
                }
                else if(input == 10) {
                    putchar(input);
                    clearTable(polybius_table);
                    index = 0;
                    input = fgetc(stdin);
                }
                else if(input == 9 && isSpace == 0) {
                    input = fgetc(stdin);
                    isSpace++;
                    putSpaceTable(polybius_table, index);
                    index = index + 2;
                }
                else if(input == 9 && isSpace != 0) {
                    input = fgetc(stdin);
                }
                //non-space input
                else {
                    isSpace = 0;
                    int diff = input - 33;
                    char morse = *(*(morse_table + diff));
                    if( morse == '\0') {
                        USAGE(*argv, EXIT_FAILURE);
                    }
                    else {
                        int morseLen = getfmLen(*(morse_table + diff));
                        putMorseTable(polybius_table, index, *(morse_table + diff), morseLen);
                        input = fgetc(stdin);
                        if(input == 32 || input == 10 || input == 9) {
                            index = index + morseLen;
                            *(polybius_table + index + 1) = '\0';
                        }
                        else {
                            index = index + morseLen + 1;
                        }
                    }
                }
                /*translate to fractionated table*/
                while(index >= 3) {

                    char m = *(polybius_table);
                    char n = *(polybius_table + 1);
                    char o = *(polybius_table + 2);
                    int j = 0;
                    while(j < 26) {
                        int result = match(*(fractionated_table + j), m, n, o);
                        if(result != 100) {
                            char encrypted = *(fm_key + j);
                            putchar(encrypted);
                            shiftTable(polybius_table, index);
                            break;
                        }
                        else {
                            j++;
                        }
                    }
                    index = index -3;
                }
            }
        }
        else {
            putchar('n');
        }
    }
}


/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */