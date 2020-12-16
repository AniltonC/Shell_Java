#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

void pathColor()
{
    printf("\033[1;34m");
}

void resetColor()
{
    printf("\033[0m");
}

void main(int argc, char *argv[])
{
    char *cmd;
    char cmdAux[4096];
    char pathAux[4096];

    pathColor();
    printf("%s", getcwd(pathAux, 4096));
    resetColor();
    printf("$ ");

    fgets(cmdAux, 4096, stdin);
    cmd = &cmdAux[0];
    cmd[strlen(cmdAux) - 1] = '\0';

    while (strcmp(cmd, "exit") != 0)
    {
        pathColor();
        printf("%s", getcwd(pathAux, 4096));
        resetColor();
        printf("$ ");
        fgets(cmdAux, 4096, stdin);
        cmd = &cmdAux[0];
        cmd[strlen(cmdAux) - 1] = '\0';
    }
}