#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include "process.h"
#include "prase.h"
#include "myshell.h"

// print a prompt
// precondition: none
// postcondition: a prompt printed
static void Prompt()
{
    printf("> ");
}

// init the shell
// precondition: none
// postcondition: the shell is initialized
void init()
{
    // the shell path
    char* path;
    // link to the path
    char* link = "/proc/self/exe";
    // the temporarily buffer size of the path
    ssize_t pathSize = 1024;
    ssize_t len = 0;
    path = (char*) malloc(sizeof(char) * pathSize);
    len = readlink(link, path, pathSize);
    // if the buffer is small
    while (len >= pathSize)
    {
        // free the space and allocate a larger one
        free(path);
        pathSize *= 2;
        path = (char*) malloc(sizeof(char) * pathSize);
    }
    fflush(stdout);
    // set the path an environment variable
    setenv("myshell", path, 1);
    // setpath("/bin:/usr/bin");
    free(path);
}

// read a command from stdin
// CMDL readcommand()
// {
//     CMDL cmdl;
//     cmdl = ReadCommand();
//     return cmdl;
// }

void Error(char* errorMessage)
{
    perror(errorMessage);
}

int main(int argc, char* argv[])
{
    CMDL cmdl;
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;
    pid_t pid;
    init();
    while (should_run)
    {
        Prompt();
        cmdl = ReadCommand();
        ExecuteCommand(cmdl);
    }
}