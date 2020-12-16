#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

	//strings
	char* cmd;
	char* arg;

	char cmdAux[4096];
	char pathAux[4096];
	char* execArg[4096];

	char* arqIn;
	char* arqOut;
	char* arqPipe;

	//forks , waits
	int wt; 
	int rc; 

	//flags
	short possibleArg = 0;
	short event;

	printf("%s$ ",getcwd(pathAux, 4096));
	fgets(cmdAux, 4096, stdin);
	cmd = &cmdAux[0];
	cmd[strlen(cmdAux)-1] = '\0';

	while(strcmp(cmd,"exit") != 0){

		if(strncmp(cmd,"cd ",3) == 0){
			arg = &cmd[3];
			int changePath = chdir(arg);
			if(changePath == -1){
				printf("cd: %s: Arquivo ou diretório inexistente\n",arg);
			}
		}else if(strcmp(cmd,"pwd") == 0){
			printf("%s\n", getcwd(pathAux, 4096)); 
		}else if(strcmp(cmd,"ls") == 0){
			rc = fork();
			if(rc < 0){
				printf("Falha ao criar o fork\n");
			}else if(rc == 0){
				execl("/bin/ls","ls",NULL,NULL);
			}else{
				wt = wait(NULL);
			}
		}else if(strcmp(cmd,"clear")==0){
			system("clear");
		}else{
			rc = fork();
			if(rc < 0){
				printf("Falha ao criar o fork\n");
			}else if(rc == 0){
				int i = 0;
				event = 0;
				possibleArg = 0;
				while((i < strlen(cmd)) && event == 0){
					if(cmd[i] == '>'){
						if(strlen(cmd) >= (i+1) + 2){
							arqOut = &cmd[i+2]; 
							cmd[i-1] = '\0';
							event = 1;
						}else{
							event = -1;
						}
					}else if(cmd[i] == '<'){
						if(strlen(cmd) >= (i+1) + 2){
							arqIn = &cmd[i+2]; 
							cmd[i-1] = '\0';
							event = 2;
						}else{
							event = -1;
						}
					}else if(cmd[i] == '|'){
						if(strlen(cmd) >= (i+1) + 2){
							arqPipe = &cmd[i+2]; 
							cmd[i-1] = '\0';
							event = 3;
						}else{
							event = -1; 
						}
					}else if(cmd[i] == ' ' && cmd[i+1] != '|' && cmd[i+1] != '>' && cmd[i+1] != '<'){
							int leghtParams = -1;
							int paramsQnt = 1;
							int memoryCmd = i;
							int CMD_LENGHT = strlen(cmd);
							arg = &cmd[i+1];
							while((i+1) < CMD_LENGHT){
								leghtParams++;
								if(cmd[i+1] == ' '){
									arg[leghtParams]='\0';
									execArg[paramsQnt] = strdup(arg);
									paramsQnt++;
									if((i+2) < CMD_LENGHT){
										arg = &cmd[i+2];
										leghtParams = -1; 
									}
								}
								i++;
							}
							execArg[paramsQnt] = strdup(arg);
							execArg[paramsQnt + 1] = NULL;	
							cmd[memoryCmd]='\0';
							event = 4;
					}
					i++;
				}
				execArg[0] = strdup(cmd);
				switch(event){
					case -1:
						printf("Invalid command\n");
					break;
					case 0:;
						execArg[1] = NULL;        
						execvp(execArg[0], execArg); 
						perror(cmd); 
					break;
					case 1:;
						close(STDOUT_FILENO);
						open(arqOut, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
						execArg[1] = NULL;        
						execvp(execArg[0], execArg); 
						perror(cmd); 
					break;
					case 2:;
						close(STDIN_FILENO);
						open(arqIn, O_RDONLY, S_IRWXU);
						execArg[1] = NULL; 		
						execvp(execArg[0], execArg); 
						perror(cmd); 					
					break;
					case 3:;
						rc = fork();
						if(rc < 0){
							printf("Fail creating fork\n");
						}else if(rc==0){
							close(STDOUT_FILENO);
							open("aux", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
							execArg[1] = NULL;
							execvp(execArg[0], execArg); 
							perror(cmd);
						}else{
							wt = wait(NULL);
							close(STDIN_FILENO);
							open("aux", O_RDONLY, S_IRWXU);
							execArg[0] = strdup(arqPipe);
							execArg[1] = NULL; 
							execvp(execArg[0], execArg); 
							perror(cmd); 
						}
					break;
					case 4:;
						execvp(execArg[0], execArg); 
						perror(cmd); 
					break;
				}
			}else{
				wt = wait(NULL);
				if(event==3){
					remove("aux");
				}
			}
		}
		printf("%s$ ",getcwd(pathAux, 4096));
		fgets(cmdAux, 4096, stdin);
		cmd = &cmdAux[0];
		cmd[strlen(cmdAux)-1] = '\0';
	}
	return 0;
}