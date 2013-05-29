#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>


void copia_file(char* argv1, char* argv2);
void copia_cartella(char *argv[], struct dirent *entry, DIR *dp);
void CreateFolder(char *dirname);


int main(int argc, char *argv[]) {

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
      entry = readdir(dp);
      lstat(entry->d_name,&statbuf);

      if(S_ISDIR(statbuf.st_mode)) 
         copia_cartella(argv,entry,dp);
   }
return 0;
}

void copia_file(char* argv1, char* argv2){   

   printf("è un file\n");


   FILE *buf_read, *buf_write;
   int ch;

   buf_read = fopen(argv1, "r");
   buf_write = fopen(argv2, "w");

   while ((ch = fgetc(buf_read)) != EOF)
      fputc(ch, buf_write);

   fclose(buf_write);
   fclose(buf_read);

   printf("Il file è stato copiato\n");
}

void copia_cartella(char *argv[], struct dirent *entry, DIR *dp){
   CreateFolder(argv[2]);
   chdir(argv[1]);

   printf("è una cartella\n");

    while((entry = readdir(dp)) != NULL) {

      printf("%s",entry->d_name);
      copia_file(entry->d_name,argv[2]);
    }
}

void CreateFolder(char *dirname){
   int check;
   check = mkdir(dirname,S_IFDIR);

   if (!check)
   printf("Directory created\n");
   else
   {
      printf("Unable to create directory\n");
      exit(1);
   }

   system("dir/p");
}

