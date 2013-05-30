#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
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
      char* argv1= (char*)malloc(sizeof(char)*500);

      argv1 = strcpy(argv1,argv[1]);
      strcat(argv1,"/");

      //printf("\n%s\n\n",argv1);

      DIR *dp;
      struct dirent *entry;
      struct stat statbuf;

      if((dp = opendir(argv[2])) == NULL) {
         copia_file(argv[2], argv[1]);
         return;
      }

      if(argv[2][strlen(argv[2])-1] != '/')
         strcat(argv[2],"/");


      copia_cartella(argv[2],argv1,dp,statbuf);

      printf("\nTutti i file sono stati copiati correttamente!\n\n");
      

   }
   return 0;
}

void copia_file(char* argv1, char* argv2){   

   if(access(argv1,R_OK)==0){

      FILE *buf_read, *buf_write;
      int ch;

      //printf("argv1: %s\n",argv1);
      //printf("argv2: %s\n",argv2);

      buf_read = fopen(argv1, "r");
      buf_write = fopen(argv2, "w");

      while((ch = fgetc(buf_read)) != EOF)
         fputc(ch, buf_write);

      fclose(buf_write);
      fclose(buf_read);
   }
   else
      printf("Il file: %s non può essere copiato\n", argv1);

   //printf("Il file è stato copiato\n");
}


void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf){


   CreateFolder(write);
   struct dirent *entry;

   //printf("A\n");
   while((entry = readdir(dp)) != NULL){
   //printf("B\n");


   if(access(read,R_OK)==0){

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

            copia_file(percorsoCartella,percorsoCartellaDest );
         }
         else{
            strcat(percorsoCartella,"/");
            strcat(percorsoCartellaDest,"/");

            //printf("Trovato cartella: %s\n",entry->d_name );
            copia_cartella(percorsoCartella,percorsoCartellaDest,dp1,statbuf1);
         }
      }
   }
}
}

void CreateFolder(char *dirname){

   int check;
   check = mkdir(dirname,0777);

  // if (!check)
  // printf("Directory created\n");

   //printf("%d\n",strcmp(percorsoInizio,percorsoDest));
}
