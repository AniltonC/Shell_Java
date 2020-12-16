#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

/////   VARIÁVEIS GLOBAIS   /////
#define BUFFERSIZE 1024
#define COPYMORE 0644

//forks , waits
int wt;
int frk;

char *cmd;
char *home;
char cmdAux[4096];
char pathAux[4096];

/////   FUNÇÕES    /////

void pathColor()
{
    printf("\033[1;34m");
}

// Altera a cor do printf para o padrão
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
    char *arg = &cmd[3];
    int removeu = remove(arg);
    if (!removeu)
    {
        printf("%s foi deletado com Sucesso!!\n", arg);
    }
    else
    {
        printf("%s não foi deletado\n", arg);
    }
}

// Altera o diretório corrente para o caminho desejado
void changePath(char *path)
{
    int change = chdir(path);
    if (change == -1)
        printf("cd: %s: No such file or directory\n", path);
}

// Avalia as variações do comando CD para alterar o diretório
void cd()
{
    char *path = &cmd[3];
    if (strcmp(path, "~") == 0)
        changePath(home);
    else
        changePath(path);
}

// Realiza a leitura dos comandos
void getCmd()
{
    char *actualPath = getcwd(pathAux, 4096);
    if (strncmp(actualPath, home, strlen(home)) == 0)
    {
        pathColor();
        printf("~%s", &actualPath[strlen(home)]);
        resetColor();
        printf("$ ");
    }
    else
    {
        pathColor();
        printf("%s", getcwd(pathAux, 4096));
        resetColor();
        printf("$ ");
    }

    fgets(cmdAux, 4096, stdin);
    cmd = &cmdAux[0];
    cmd[strlen(cmdAux) - 1] = '\0';
}

// void log()
// {
//     char *command = cmd;
//     strcat(command, "\n");
//     FILE *f;
//     f = fopen("shell_AG.log", "a+"); // a+ (create + append) option will allow appending which is useful in a log file
//     if(f != NULL)
//         fprintf(f, *command);
// }

void main()
{
    home = getenv("HOME");

    getCmd();
    while (strcmp(cmd, "exit") != 0)
    {
        // log();

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
        else if (strncmp(cmd, "cd ", 3) == 0)
        {
            cd();
        }
        else if (strcmp(cmd, "clear") == 0)
        {
            system("clear");
        }
        else
        {
            system(cmd);
        }
        
        getCmd();
    }
}