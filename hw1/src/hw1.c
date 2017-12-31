#include "hw1.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the program
 * and will return a unsigned short (2 bytes) that will contain the
 * information necessary for the proper execution of the program.
 *
 * IF -p is given but no (-r) ROWS or (-c) COLUMNS are specified this function
 * MUST set the lower bits to the default value of 10. If one or the other
 * (rows/columns) is specified then you MUST keep that value rather than assigning the default.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return Refer to homework document for the return value of this function.
 */

unsigned short validargs(int argc, char **argv) {
    unsigned short mod = 0x0000;
    short rckFlag = 0x0;
    int row = 0;
    int col = 0;
    int i = 2;
    int polyLen = getLength(polybius_alphabet);
    int fmLen = getfmLen(fm_alphabet);
    if(argc == 1) {
        return mod;
    }
    else {
        char *dash = *(argv + 1);
        if(*(dash + 1) == 'h') {
            return 0x8000;
        }
        else if(argc == 2) {
            return 0x0000;
        }
        else if(*(dash + 1) == 'p') {
            if(argc > 9) {
                return 0x0000;
            }
            while(i<argc) {
                char *currArg = *(argv + i);
                if(i == 2) {
                    if(*(currArg + 1) == 'e') {
                        if(argc == 3) {
                            /*p->e*/
                            return 0x00AA;
                        }
                    }
                    else if(*(currArg + 1) == 'd') {
                        if(argc == 3) {
                            /*p->d*/
                            return 0x20AA;
                        }
                        else {
                            mod = mod + 8192;
                        }
                    }
                    else {
                        return 0x0000;
                    }
                }
                /*Third argument and on..*/
                else {
                    if(*(currArg + 1) == 'e' || *(currArg + 1) == 'd' || *(currArg + 1) == 'p') {
                        return 0x0000;
                    }
                    else if(*(currArg + 1) == 'k'){
                        if((rckFlag & 0x8) != 0x0) {
                            return 0x0000;
                        }
                        else {
                            int valid = isValid(*(argv + i + 1), polyLen);
                            if(valid == 1) {
                                key = *(argv + i + 1);
                                i++;
                            }
                            else {
                                return 0x0000;
                            }
                        }
                    }
                    else if(*(currArg + 1) == 'r') {
                        if((rckFlag & 0x4) == 0x0) {
                            sscanf(*(argv + i + 1), "%d", &row);
                            if(row > 16 || row < 9) {
                                return 0x0000;
                            }
                            else {
                                mod = mod + (row*16);
                                i++;
                                rckFlag = rckFlag + 4;
                            }
                        }
                        else {
                            return 0x0000;
                        }
                    }
                    else if(*(currArg + 1) == 'c') {
                        if((rckFlag & 0x2) == 0x0) {
                            sscanf(*(argv + i + 1), "%d", &col);
                            if(col > 16 || col < 9) {
                                return 0x0000;
                            }
                            else {
                            mod = mod + col;
                            i++;
                            rckFlag = rckFlag + 2;
                            }
                        }
                        else {
                            return 0x0000;
                        }
                    }
                    else {
                        return 0x0000;
                    }
                }
                i++;
            }
            if((rckFlag & 0x2) == 0x0) {
                mod = mod + 0x000A;
                col = 10;
            }
            if((rckFlag & 0x4) == 0x0) {
                mod = mod + 0x00A0;
                row = 10;
            }
            if(row*col < polyLen) {
                return 0x0000;
            }
            return mod;
        }
        else if(*(dash + 1) == 'f') {
            mod = mod + 16384;
            if(argc > 5) {
                return mod;
            }
            char *currArg = *(argv + 2);
            if(*(currArg + 1) == 'e') {
                mod = mod + 0;
                if(argc == 3) {
                    /*f->e*/
                    return 0x4000;
                }
            }
            else if(*(currArg + 1) == 'd') {
                mod = mod + 8192;
                if(argc == 3) {
                    /*f->d*/
                    return 0x6000;
                }
            }
            else {
                return 0x0000;
            }
            if(argc > 3) {
                currArg = *(argv + 3);
                if(*(currArg + 1) == 'e' || *(currArg + 1) == 'd' || *(currArg + 1) == 'f') {
                        return 0x0000;
                }
                else if(*(currArg + 1) == 'k'){
                    if(rckFlag & 0x8) {
                            return 0x0000;
                    }
                    else {
                        int valid = isFMvalid(*(argv + 4), fmLen);
                        if(valid == 1) {
                            key = *(argv + 4);
                            i++;
                        }
                        else {
                            return 0x0000;
                        }
                    }
                }
                else {
                    return 0x0000;
                }

            }
            return mod;
        }
        else {
            return mod;
        }
    }
}

int isRepeated(char *k) {
    int keyLen = getLength(k);
    int i = 0;
    int j = 0;
    while(i < keyLen) {
        char *currLetter = k + i;
        j = i + 1;
        while(j < keyLen) {
            char *nextLetter = k + j;
            if(*currLetter == *nextLetter) {
                return 0;
            }
            else {
                j++;
            }
        }
        i++;
    }
    return 1;
}

int isValid(char *k, int Length) {
    int i = 0;
    int j = 0;
    int found = 1;
    int keyLen = getLength(k);
    int result = isRepeated(k);
    if(result == 0) {
        return 0;
    }
    else {
        while(i < keyLen) {
            char *keyLetter = k + i;
            j = 0;
            found = 0;
            while(j < Length) {
                char *alphaLetter = polybius_alphabet + j;
                if(*keyLetter == *alphaLetter) {
                    found = 1;
                    break;
                }
                else {
                    j++;
                }
            }
            if(found == 0) {
                return 0;
            }
            else {
                i++;
            }
        }
    }
    return 1;
}

int isFMvalid(char *k, int Length) {
    int i = 0;
    int j = 0;
    int found = 1;
    int keyLen = getLength(k);
    if(isRepeated(k) == 0) {
        printf("%s\n", "Repeated characters!");
        return 0;
    }
    else {
        while(i < keyLen) {
            char *keyLetter = k + i;
            j = 0;
            found = 0;
            while(j < Length) {
                char const *alphaLetter = fm_alphabet + j;
                if(*keyLetter == *alphaLetter) {
                    found = 1;
                    break;
                }
                else {
                    j++;
                }
            }
            if(found == 0) {
                return 0;
            }
            else {
                i++;
            }
        }
    }
    return 1;
}

int getLength(char *alphabet) {
    int i = 0;
    while(*(alphabet + i) != 0) {
        i++;
    }
    return i;
}

int getfmLen(const char *alphabet) {
    int i = 0;
    while(*(alphabet + i) != 0) {
        i++;
    }
    return i;
}
