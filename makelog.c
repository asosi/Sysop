#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "makelog.h"

void initlog(char* app){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	chdir("/var/log/");
	mkdir("utility", S_IRWXU|S_IRGRP|S_IXGRP);
	chdir("utility");
	chmod("/var/log/utility", 0777);
	
	char aggiusta[1024];
	int k= 2;
	strcpy(aggiusta,app+2);
	mkdir(aggiusta, 0777);
    chdir(aggiusta);
    char file[1024];
    strcpy(file,aggiusta);
    strcat(file,".log");
    FILE *openr = fopen(file,"r");
    if(openr == NULL){
    	printf("Creato il file di log %s in /var/log/utility/%s\n", file, aggiusta);
    	printf("%s\n",file);
    	FILE *openw = fopen(file,"a");
    	fclose(openw);
    	chmod(file, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		
    }
    else
    	fclose(openr);
    chdir(cwd);
}

void writeERROR(char* app, char *ERROR){
	initlog(app);
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	char nome[1024];
	chdir("/var/log/utility");
	chdir(app);
	char aggiusta[1024];
	int k= 2;
	strcpy(aggiusta,app+2);
	strcpy(nome,aggiusta);
	strcat(nome,".log");
	FILE *open = fopen(nome,"a");
	time_t ora = time(NULL);
	char ERRORE[1024];
	fputs(asctime(localtime(&ora)),open);
	strcpy(ERRORE,"ERROR: ");
	strcat(ERRORE, "   ");
	strcat(ERRORE, ERROR);
	strcat(ERRORE,"\n");
	fputs(ERRORE,open);
	fputs("************************************************************\n",open);
	fclose(open);	
}