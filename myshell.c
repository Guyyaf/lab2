#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "LineParser.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <signal.h>

#define MAX_BUFFER 2048
int debug = 0;

void debugCheck(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] == 'd' && argv[i][2] == '\0')
        {
            debug = 1;
        }
    }
}

void cwdPrompt()
{
    char cwdBuffer[PATH_MAX];
    getcwd(cwdBuffer, PATH_MAX);
    printf("%s ", cwdBuffer);
}

void alarmFunc(int pid)
{
    if (kill(pid, SIGCONT) != 0)
    {
        perror("unsuccesfull alarm");
    }
    else
    {
        printf("Woken pid: %d\n", pid);
    }
}

void blastFunc(int pid)
{
    if (kill(pid, SIGINT) != 0)
    {
        perror("unsuccesfull blast");
    }
    else
    {
        printf("Terminated pid: %d\n", pid);
    }
}

void execute(cmdLine *pCmdLine)
{
    int pid;
    if (strcmp(pCmdLine->arguments[0], "cd") == 0 && pCmdLine->arguments[1] != NULL)
    {
        if (chdir(pCmdLine->arguments[1]) == -1)
        {
            fprintf(stderr, "cd: %s: No such directory\n", pCmdLine->arguments[1]);
        }
        return;
    }
    if (strcmp("alarm", pCmdLine->arguments[0]) == 0)
    {
        alarmFunc(atoi(pCmdLine->arguments[1]));
        return;
    }
    if (strcmp("blast", pCmdLine->arguments[0]) == 0)
    {
        blastFunc(atoi(pCmdLine->arguments[1]));
        return;
    }
    if ((pid = fork()) == 0)
    {
        if (pCmdLine != NULL)
        {
            if (pCmdLine->inputRedirect != NULL)
            {
                int newInputFile = open(pCmdLine->inputRedirect, O_RDONLY);
                if (dup2(newInputFile, 0) == -1)
                {
                    perror("failed switching stdin");
                    exit(1);
                }
                close(newInputFile);
            }
            if (pCmdLine->outputRedirect != NULL)
            {
                int newOutputFile = open(pCmdLine->outputRedirect, O_WRONLY);
                if (dup2(newOutputFile, 1) == -1)
                {
                    perror("failed switching stdout");
                    _exit(1);
                }
                close(newOutputFile);
            }
            if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1)
            {
                perror("execvp failed");
                exit(1);
            }
        }
    }
    else
     {
            if (debug)
            {
                fprintf(stderr, "PID: %d\n", pid);
                fprintf(stderr, "Executing command: %s\n", pCmdLine->arguments[0]);
            }
            if (pCmdLine->blocking == 1)
            {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }


int main(int argc, char *argv[])
{
    char inputBuffer[MAX_BUFFER];
    cmdLine *input;
    debugCheck(argc, argv);
    while (1)
    {
        cwdPrompt();
        fgets(inputBuffer, MAX_BUFFER, stdin);
        input = parseCmdLines(inputBuffer);
        if (strcmp(input->arguments[0], "quit") == 0)
        {
            freeCmdLines(input);
            break;
        }
        execute(input);
        freeCmdLines(input);
    }
    return 0;
}
