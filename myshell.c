#include "LineParser.c"
#include <limits.h>
#include <unistd.h>

#define MAX_INPUT 2048 

void execute(cmdLine *pCmdLine) {
    int pid;
    if ((pid = fork()) == 0) { 
        if (execv(pCmdLine->arguments[0], pCmdLine->arguments) == -1) {
            perror("execvp failed");
            exit(1);
        }
    } else { 
        waitpid(pid, NULL, 0); //coppied line from "chat-gtp"
    }
}

int main(int argc, char *argv[]) {
    char cwdBuffer[PATH_MAX];
    char inputBuffer[MAX_INPUT];
    cmdLine* input;
    while(1==1){
        getcwd(cwdBuffer, PATH_MAX);
        printf("Current working directory is - %s\n", cwdBuffer);
        fgets(inputBuffer,MAX_INPUT,stdin);
        input = parseCmdLines(inputBuffer);
        execute(input);
        freeCmdLines(input);
    }
    return 0;    
}

