#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include "sfish.h"
#include "debug.h"

char *cwd;
char *cwdBuffer;
char *buffer;
char *oldcwd;
char **arguments;
pid_t pid;
int argNum;

int main(int argc, char *argv[], char* envp[]) {
    char* input;
    bool exited = false;
    cwdBuffer = calloc(1024, 1);
    buffer = calloc(1024,1);
    oldcwd = calloc(1024,1);
    int homeLen = strlen(getenv("HOME"));
    char home[homeLen];

    if(!isatty(STDIN_FILENO)) {
        // If your shell is reading from a piped file
        // Don't have readline write anything to that file.
        // Such as the prompt or "user input"
        if((rl_outstream = fopen("/dev/null", "w")) == NULL){
            perror("Failed trying to open DEVNULL");
            exit(EXIT_FAILURE);
        }
    }

    do {

        cwd = getcwd(cwdBuffer, 1024);

        memcpy(home, cwd, homeLen);

        if(strcmp(home, getenv("HOME")) == 0) {
            buffer[0]='\0';
            strcat(buffer, "~");
            strcat(buffer, cwd+homeLen);
        }
        else {
            strcpy(buffer, cwd);
        }

        strcat(buffer, " :: qianyang >> ");
        char *newcwd = buffer;
        input = readline(newcwd);

        argNum = getInputArgNum(input);

        // write(1, "\e[s", strlen("\e[s"));
        // write(1, "\e[20;10H", strlen("\e[20;10H"));
        // write(1, "SomeText", strlen("SomeText"));
        // write(1, "\e[u", strlen("\e[u"));

        /* process input. */
        // If EOF is read (aka ^D) readline returns NULL
        if(strcmp(input, "") == 0 || (input[0] == ' ' && argNum == 0)) {
            continue;
        }
        else {
            builtinCommands(input);
        }

        // Readline mallocs the space for input. You must free it.
        rl_free(input);

    } while(!exited);

    debug("%s", "user entered 'exit'");

    return EXIT_SUCCESS;
}

void builtinCommands(char *input) {
    arguments = parseInput(input);
    if(argNum == 1) {
        if(strcmp(arguments[0], "help") == 0 ) {
            printHelp();
        }
        else if(strcmp(arguments[0], "exit") == 0) {
            exit(EXIT_SUCCESS);
        }
        else if(strcmp(arguments[0], "pwd") == 0) {
            cwd = getcwd(cwdBuffer, 1024);
            printf("%s\n", cwd);
        }
        else if(strcmp(arguments[0], "cd") == 0) {
            strcpy(oldcwd, cwd);
            chdir(getenv("HOME"));
        }
        else {
            pid = fork();
            if(pid == 0) {
                //child
                if(execvp(arguments[0], arguments) < 0) {
                    fprintf(stderr, EXEC_ERROR, input);
                    exit(0);
                }
            }
            else {
                //parent.
                int status;
                if(waitpid(pid, &status, 0) < 0) {
                    fprintf(stderr, EXEC_ERROR, input);
                }
            }
        }
    }
    else {
        char *temp = calloc(1024,1);
        strcpy(temp,input);
        char *first_arg = strtok(temp, " |<>");
        if(strcmp(first_arg, "cd") == 0) {
            //cd -
            //char *temp_d = cwd;
            char *next_arg = strtok(NULL, " |<>");
            if(strcmp(next_arg, "-") == 0) {
                if(oldcwd == NULL) {
                    fprintf(stderr, BUILTIN_ERROR, input);
                }
                else {
                    cwd = getcwd(cwdBuffer, 1024);
                    char* x = cwd;
                    chdir(oldcwd);
                    strcpy(oldcwd, x);
                    printf("%s\n", cwd);

                }
            }
            else if(strcmp(next_arg, ".") == 0) {
                chdir(cwd);
                strcpy(oldcwd, cwd);
            }
            else {
                if(strstr(next_arg, "/") != NULL) {
                    if(chdir(next_arg) == -1) {
                        fprintf(stderr, BUILTIN_ERROR, input);
                    }
                    else {
                    }
                }
                else {
                    next_arg = strcat(next_arg, "/");
                    if(strstr(next_arg, "/") != NULL) {
                        if(chdir(next_arg) == -1) {
                            fprintf(stderr, BUILTIN_ERROR, input);
                        }
                        else {
                        }
                    }

                }
            }
        }
        else {
            /****************** PART TWO *********************/
            pid = fork();
            arguments = parseInput(input);
            if(pid == 0) {
                //child
                if(strstr(input, "|") != NULL || strstr(input, "<") != NULL || strstr(input, ">") != NULL) {
                        redirect(arguments, argNum);
                        exit(0);
                }
                else  {
                    if(execvp(arguments[0], arguments) < 0) {
                        fprintf(stderr, EXEC_ERROR, *arguments);
                        exit(0);
                    }
                }
            }
            else {
                //parent.
                int status;
                if(waitpid(pid, &status, 0) < 0) {
                    fprintf(stderr, EXEC_ERROR, *arguments);
                }
            }
        }
    }
}

void printHelp() {
    printf("%s\n", "These shell commands are defined internally.  Type `help' to see this list.");
    printf("%s\n", "help : prints a list of all buitins and their basic usage.");
    printf("%s\n", "exit : exits shell.");
    printf("%s\n", "pwd : prints absolute path of the current directory.");
    printf("%s\n", "cd : change current directory to some directory.");
    printf("%s\n", "    -> cd - : changes current directory to the last directory.");
    printf("%s\n", "    -> cd . : current directory");
    printf("%s\n", "    -> cd .. : change current directory to previous directory");
    printf("%s\n", "    -> cd : changes current directory to home directory");


}

int getInputArgNum(char *input) {
    char *temp = calloc(1024,1);
    strcpy(temp,input);
    char *curr_arg = strtok(temp, " ");
    int argNum = 0;
    while(curr_arg != NULL) {
        argNum ++;
        curr_arg = strtok(NULL, " ");

    }
    free(temp);
    return argNum;
}

char **parseInput(char *input) {
    char *temp = calloc(1024,1);
    strcpy(temp,input);
    char **arguments = calloc(1024, 1);
    int i = 0;
    char *token = strtok(temp, " ");
    while(token != NULL) {
        arguments[i] = token;
        i++;
        token = strtok(NULL, " ");
    }

    return arguments;
}

void redirect(char **argument, int argNum) {
    char *delim;
    char path[1024];
    delim = "<";
    int intoIndex = findIndex(argument, argNum, delim);
    delim = ">";
    int outoIndex = findIndex(argument, argNum, delim);
    delim = "|";
    int pipeIndex = findIndex(argument, argNum, delim);

    char **newArgBuffer;

    if(intoIndex == -1 && outoIndex == -1) {
        executePipes(cwd, argument[0]);

    }
    else if(pipeIndex == -1 && outoIndex == -1) {
        newArgBuffer = makeNewArg(argument, intoIndex);
        char *file = argument[intoIndex+1];
        strcpy(path, cwd);
        strcat(path, "/");
        strcat(path, file);
        //printf("%s\n", path);
        int fd = open(path, O_RDONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(fd < 0) {
            printf("%s\n", "FILE NOT EXISTED");
        }
        else {
            int oldfd = dup(STDIN_FILENO);
            dup2(fd, STDIN_FILENO);
            execvp(argument[0], newArgBuffer);
            dup2(oldfd, STDIN_FILENO);
        }
    }
    else if(pipeIndex == -1 && intoIndex == -1) {

        newArgBuffer = makeNewArg(argument, outoIndex);
        char *file = argument[outoIndex+1];
        strcpy(path, cwd);
        strcat(path, "/");
        strcat(path, file);
        printf("%s\n", path);
        int fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(fd < 0) {
            printf("%s\n", "FILE NOT EXISTED");
        }
        else {
            int oldfd = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            execvp(argument[0], newArgBuffer);
            dup2(oldfd, STDOUT_FILENO);
        }
    }
}

int findIndex(char **argument, int argNum, char *delim) {
    for(int i = 0; i<argNum; i++) {
        if(strcmp(argument[i], delim) == 0) {
            return i;
        }
        else if(strcmp(argument[i], delim) == 0) {
            return i;
        }
    }
    return -1;
}

char **makeNewArg(char **argument, int index) {
    char **newArgBuffer = calloc(1024, 1);
    for(int i = 0; i<index; i++) {
        newArgBuffer[i] = argument[i];
    }
    return newArgBuffer;
}

void executePipes(char* command, char* userPut) {
    printf("%s\n", "NOT YET IMPLEMENTED.");

}


