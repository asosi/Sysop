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
	if (mkdir(app, S_IRWXU|S_IRGRP|S_IXGRP) != 0){
    	perror("FATAL ERROR");
    	exit(EXIT_FAILURE);
    }
    chdir(app);
    char file[1024];
    strcat(file,app);
    strcat(file,".log");
    FILE *open = fopen(file,"r");
    if(open == NULL){
    	printf("Creato il file di log %s in /var/log/utility/%s\n", file, app);
    	fclose(open);
    	FILE *open = fopen(file,"a");
    	fclose(open);
    }
    else
    	fclose(open);
    chdir(cwd);
}

void writeERROR(char* app, char *ERROR){
	initlog(app);
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	char nome[1024];
	chdir("/var/log/utility");
	chdir(app);
	strcpy(nome,app);
	strcat(nome,".log");
	FILE *open = fopen(nome,"a");
	time_t ora = time(NULL);
	char ERRORE[1024];
	strcat(ERRORE,asctime(localtime(&ora)));
	strcat(ERRORE, "   ");
	strcat(ERRORE, ERROR);
	fputs(ERRORE,open);
	fclose(open);	
}