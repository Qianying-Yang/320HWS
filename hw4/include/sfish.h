#ifndef SFISH_H
#define SFISH_H

/* Format Strings */
#define EXEC_NOT_FOUND "sfish: %s: command not found\n"
#define JOBS_LIST_ITEM "[%d] %s\n"
#define STRFTIME_RPRMT "%a %b %e, %I:%M%p"
#define BUILTIN_ERROR  "sfish builin error: %s\n"
#define SYNTAX_ERROR   "sfish syntax error: %s\n"
#define EXEC_ERROR     "sfish exec error: %s\n"


void builtinCommands(char *input);
void printHelp();
int getInputArgNum(char *input);
char **parseInput(char *input);
void redirect(char **argument, int argNum);
void executePipes(char* command, char* userPut);
int findIndex(char **argument, int argNum, char *delim);
char **makeNewArg(char **argument, int index);
void executePipes(char* command, char* userPut);



#endif
