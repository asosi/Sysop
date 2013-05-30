#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#define __USE_GNU

void copia_file(char* argv1, char* argv2);
void copia_cartella(char *read, char *write, DIR *dp, struct stat statbuf);
void CreateFolder(char *dirname);

int main(int argc, char *argv[]){

if(argc < 3){
      printf("Mancano argomenti! Passare due file o due cartelle!\n");
      //exit
   }  
   else if(argc > 3)
      printf("Troppi argomenti! Passare due file o due cartelle!\n");
      //exit
   else{


	   DIR *dp;
	   struct dirent *entry;
	   struct stat statbuf;

	   if((dp = opendir(argv[1])) == NULL) {
	        copia_file(argv[1], argv[2]);
	        return;
	   }

	   copia_cartella(argv[1],argv[2],dp,statbuf);
      

   }
   return 0;
}

void copia_file(char* argv1, char* argv2){   

   printf("è un file\n");

   FILE *buf_read, *buf_write;
   int ch;

   printf("argv1: %s\n",argv1);
   printf("argv2: %s\n",argv2);

   buf_read = fopen(argv1, "r");
   buf_write = fopen(argv2, "w");

   while((ch = fgetc(buf_read)) != EOF)
      fputc(ch, buf_write);

   fclose(buf_write);
   fclose(buf_read);

   printf("Il file è stato copiato\n");
}


void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf){
   
   CreateFolder(write);
   printf("è una cartella\n");

   //char* percorsoCartella = argv[1];
   //char* percorsoCartellaDest = argv[2];

   struct dirent *entry;

   printf("A\n");
   while((entry = readdir(dp)) != NULL){
   printf("B\n");
   

      DIR *dp1;
      struct stat statbuf1;

      if(strcmp(".",entry->d_name)!=0 && strcmp("..",entry->d_name)!=0 && entry->d_name[0] !='.')
      {

         char* percorsoCartella = (char*)malloc(sizeof(char)*500);
         char* percorsoCartellaDest = (char*)malloc(sizeof(char)*500);

         percorsoCartella = strcpy(percorsoCartella,read);
         percorsoCartellaDest = strcpy(percorsoCartellaDest,write);
         printf("file: %s\n",entry->d_name);


         strcat(percorsoCartella,entry->d_name);
         strcat(percorsoCartellaDest,entry->d_name);

         lstat(percorsoCartella,&statbuf1);
         
         //printf("%d\n", S_ISDIR(statbuf1.st_mode));
         dp1 = opendir(percorsoCartella);
         


         if(!S_ISDIR(statbuf1.st_mode)){
            //printf("%s\n", percorsoCartella);
            //printf("%s\n", percorsoCartellaDest);


            //printf("%s\n", strcat(percorsoCartella,entry->d_name));
            //printf("%s\n", strcat(percorsoCartellaDest,entry->d_name));
            copia_file(percorsoCartella,percorsoCartellaDest );
         }
         else{
            strcat(percorsoCartella,"/");
            strcat(percorsoCartellaDest,"/");

            printf("Trovato cartella: %s\n",entry->d_name );
            copia_cartella(percorsoCartella,percorsoCartellaDest,dp1,statbuf1);
         }
      }
   }
}

void CreateFolder(char *dirname){

   int check;
   check = mkdir(dirname,0777);

   if (!check)
   printf("Directory created\n");

   //printf("%d\n",strcmp(percorsoInizio,percorsoDest));
}
