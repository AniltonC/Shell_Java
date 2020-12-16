#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

void cd()
{
    char *diretorio = strtok(NULL, " ");

    if (chdir(diretorio) != 0)
    {
        perror("");
    }
}

void ls()
{
    system("ls");
}

void pwd()
{
    char *pwd = malloc(sizeof(char) * 300);
    printf("\n %s \n\n", getcwd(pwd, 300));
    free(pwd);
}




void exec_arquivo(char *linha_comando)
{

    if (strlen(linha_comando) > 0)
    {

        int pfilho = fork();

        if (pfilho < 0)
        {
            exit(1);
            return;
        }
        else if (pfilho == 0)
        {
            char filename[500];
            char *aux;
            char *args[200];
            char linha[200];
            char entrada2[500];
            int i = 0;
            strcpy(filename, linha_comando);
            aux = strtok(linha_comando, " ");

            int saida = 0;
            int entrada = 0;
            int out = 0;
            while (aux != NULL)
            {

                if(!strcmp(aux,"&")){
                    aux = strtok(NULL, " ");
                    strcpy(aux, aux);
                }
                if (!strcmp(aux, ">"))
                { // /.prog1 > saída =  roda prog1 e joga o output dele para o arquivo saída
                    aux = strtok(NULL, " ");
                    strcpy(filename, aux);
                    saida = 1;
                }
                if (!strcmp(aux, "<"))
                { // ./prog1 < entrada =  roda prog1 e recebe como input o arquivo entrada
                    aux = strtok(NULL, " ");
                    strcpy(filename, aux);
                    entrada = 1;
                }
                if (!strcmp(aux, "cd"))
                {
                    cd();
                    out = 1;
                }

                if (!strcmp(aux, "ls"))
                {
                    ls();
                    out = 1;
                }

                if (!strcmp(aux, "pwd"))
                {
                    pwd();
                    out = 1;
                }

                args[i] = aux;
                i++;
                aux = strtok(NULL, " ");
            }
            if (saida == 1)
            {
                int rc = fork();
                if (rc < 0)
                {
                    fprintf(stderr, "Error\n");
                    exit(1);
                }
                else if (rc == 0)
                {
                    close(STDOUT_FILENO);
                    open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
                    args[i++] = NULL;
                    execvp(args[0], args);
                }
            }
            else if(out == 0)
            {
                args[i++] = NULL;
                execvp(args[0], args);

            }
        }
        else
        {
            wait(NULL);
        }
    }
}

void exec_pipi(char *args1, char *args2){
	int pip[2];
	int pstat = pipe(pip);
    //printf("no pipe %s \n",args1[0]);
	if(pstat < 0){
        fprintf(stderr, "Error\n");
		exit(1);
	}

	int rc = fork();

	if(rc < 0){
        fprintf(stderr, "Error\n");
		exit(1);
	}else if(rc == 0){
        strcat(args1," > saida");
		exec_arquivo(args1);
		exit(1);
	}else{
		wait(NULL);
        strcat(args2," < saida");
		exec_arquivo(args2);
		//exit(0);
	}
    return;
}



void pre(char *linha_de_comando){
    int i = 0;
    int p = 0;
    int j = 0;
    int pipe = 0;
    char *aux;
    char ant[500];
    char dps[500];
	char  args[500];
	char  pargs[500];
    aux = strtok(linha_de_comando, " ");

    while(aux!=NULL){
        //printf("ant: %s no i%d  aux:%s  \n",ant,i,aux);
        if(!strcmp(aux,"|")){
			p = 1;
            j = 0;
            pipe = 1;
        }else if(!strcmp(aux,"&")){
            //printf("erro entrou &zn");
            i = -1;
        }else if(i==-1){
            //printf("erro entrou --)\n");
            strcpy(dps,aux);
            strcat(dps," ");
            i = 2;
        }else if(i == 2){
            strcat(dps,aux);
            strcat(dps," ");
        }else if(i == 0){
            strcpy(ant,aux);
            strcat(ant," ");
            //printf("if igual a zero ant: %s no i%d  aux:%s  \n",ant,i,aux);
            i = 3;
            i++;
        }else{
            strcat(ant, aux);
            strcat(ant," ");
        }
        if((i!=-1) && (p == 0)){
            if( j == 0){
                strcpy(args,aux);
                strcat(args," ");
                j = 1;
            }else{
                strcat(args, aux);
            }
        }if((i!=-1) && (p == 1)){
            if( j == 0){
                strcpy(pargs,aux);
                strcat(pargs," ");
                j = 1;
            }else{
                strcat(pargs, aux);
            }
        }
        //printf("aqui dentro args %s\n",args[p-1]);
        aux = strtok(NULL, " ");
    }

    if(pipe == 1){
        //printf("entrou)\n");
        //printf("pargs %s  args %s \n,",pargs,args);
        exec_pipi(pargs,args);
        return;
    }
    if(i >= 3){
        exec_arquivo(ant);
    }
    if(i == 2){

        exec_arquivo(ant);
        exec_arquivo(dps);
    }

    return;

}

int main()
{

    printf("Proto terminal 1.0v\n");
    char entrada[500];
    char dir[300];

    //int is_running = 1;

    while (printf("%s :~  ", getcwd(dir, 300)))
    {
        scanf("%[^\n]%*c", entrada);
        pre(entrada);
    }

    return 0;
}
