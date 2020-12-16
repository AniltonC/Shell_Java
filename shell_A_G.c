#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/////   VARIÁVEIS GLOBAIS   /////

//forks , waits
int wt;
int frk;

char *cmd;
char cmdAux[4096];
char pathAux[4096];

/////   FUNÇÕES    /////

void pathColor()
{
    printf("\033[1;34m");
}

void resetColor()
{
    printf("\033[0m");
}

/**
 * Função que é chamada quando o usuário digita "ls"
 * ela é respondavel 
 * */
void ls()
{
    frk = fork();
    if (frk < 0)
    {
        /** 
         * Se o fork retornar um valor menor que 0 quer dizer que ocorreu 
         * algum erro na criação do processo filho.
         * */
        printf("Falha ao criar o fork\n");
    }
    else if (frk == 0)
    {
        /** 
         * Se o fork retornar 0 quer dizer que o código foi executado pelo
         * filho.
         * */
        execl("/bin/ls", "ls", NULL, NULL);
    }
    else
    {
        /** 
         * Se o fork retornar um valor maior que 0 quer dizer que o filho
         * está em execução portanto o pai deve esperar o retorno do filho.
         * */
        wt = wait(NULL);
    }
}

/**
 * Função que é chamada quando o usuário digita "pwd"
 * ela printa no terminal o resultado da função getcwd()
 * e esta por sua vez retorna o nome do diretório atual.
 * */
void pwd()
{
    char pathAux[4096];
    printf("%s\n", getcwd(pathAux, 4096));
}

void rm()
{
    char* arg = &cmd[3];
    int removeu = remove(arg);
    if (!removeu)
    {
        printf("%s foi deletado com Sucesso!!\n", arg);
    }
    else
    {
        printf("%s não foi deletado\n",arg);
    }
    
}

/////   MAIN    /////

void main(int argc, char *argv[])
{
    pathColor();
    printf("%s", getcwd(pathAux, 4096));
    resetColor();
    printf("$ ");

    fgets(cmdAux, 4096, stdin);
    cmd = &cmdAux[0];
    cmd[strlen(cmdAux) - 1] = '\0';

    while (strcmp(cmd, "exit") != 0)
    {
        if (strcmp(cmd, "pwd") == 0)
        {
            pwd();
        }
        else if (strcmp(cmd, "ls") == 0)
        {
            ls();
        }
        else if (strncmp(cmd, "rm ", 3) == 0)
        {
            rm();
        }

        pathColor();
        printf("%s", getcwd(pathAux, 4096));
        resetColor();
        printf("$ ");
        fgets(cmdAux, 4096, stdin);
        cmd = &cmdAux[0];
        cmd[strlen(cmdAux) - 1] = '\0';
    }
}