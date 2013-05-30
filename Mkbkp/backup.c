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


//***********************************HEADER***********************************
void CreaArchivio(char* percorsoDest, char* percorso);
void EstraiArchivio();
void VisualizzaArchivio();

void copia_file(char* argv1, char* argv2);
void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf);
void StampaBKP(char k,char* percorsoDest);
//****************************************************************************


char* inizio = "--------------------------------------------------------------------inizio----------------------------------------------------------------\n";
char* fine = "--------------------------------------------------------------------fine--------------------------------------------------------------------";
char* riga = "\n--------------------------------------------------------------------------------------------------------------------------------------------\n";

int main(int argc, char *argv[]){

	char ch;
	char* percorsoDest = NULL;
	char* percorso = NULL;
	int c = 0, x = 0, t = 0;

	while ((ch = getopt (argc, argv, "f: cxt")) != -1){
        switch (ch){
        	case 'f': percorsoDest = optarg; percorso = argv[optind];  break; //-f <archivio> indica l'archivio da creare od estrarre. 
        	case 'c': c = 1; break; 			//-c indica la creazione dell'archivio
        	case 'x': x = 1; break; 			//-x indica l'estrazione dell'archivio nella directory corrente
        	case 't': t = 1; break; 			//-t indica che deve essere visualizzato un elenco del contenuto dell'archivio
        }
    }

    //printf("%s\n", percorso);
    //printf("%s\n", percorsoDest);

    if(percorso == NULL && c == 1)
    	printf("Non hai inserito l'archivio da creare\n");
    else if(percorso == NULL && x == 1)
    	printf("Non hai inserito l'archivio da estrarre\n");    
    else if(percorso == NULL && t == 1)
    	printf("Non hai inserito l'archivio da visualizzare\n");
    else{
    	if(c == 1){ 

			StampaBKP('0',percorsoDest);

			char* argv1= (char*)malloc(sizeof(char)*500);

	      	argv1 = strcpy(argv1,argv[1]);
		    strcat(argv1,"/");

		    //printf("\n%s\n\n",argv1);

		    DIR *dp;
		    struct dirent *entry;
		    struct stat statbuf;

		   
         	if(!S_ISDIR(statbuf.st_mode)){
    			CreaArchivio(percorsoDest,percorso);
		        return;
		    }
		    else{
			    if(argv[2][strlen(argv[2])-1] != '/')
			        strcat(argv[2],"/");
				copia_cartella(argv[2],argv1,dp,statbuf);
		    }
    	}
    	else if(x == 1) EstraiArchivio();
    	else if(t == 1); VisualizzaArchivio();
    }

}

void CreaArchivio(char* percorsoDest, char* percorso){
	printf("Creazione dell'archivio:\n");


	StampaBKP('i',percorsoDest); //inizio
	StampaBKP('n',percorso); //percorso
	StampaBKP('r',percorsoDest); //riga
	
	copia_file(percorso,percorsoDest); //contenuto file

	StampaBKP('-',percorsoDest); //riga vuota
	StampaBKP('f',percorsoDest); //fine

}

void StampaBKP(char k,char* percorso){
	FILE *buf_write;
	if(k!='0')
    	buf_write = fopen(percorso, "a");
    else
    	buf_write = fopen(percorso, "w");

    char ch;

    switch(k){
    	case '0': fputs("",buf_write); break;
    	case 'i': fputs(inizio,buf_write); break;
    	case 'f': fputs(fine,buf_write); break;
    	case 'r': fputs(riga,buf_write); break;
    	case 'n': fputs(percorso,buf_write); break;
    	case '-': fputc('\n',buf_write);break;
      }
	fclose(buf_write);
}

void copia_file(char* argv1, char* argv2){   

	if(access(argv1,R_OK)==0){

    	FILE *buf_read, *buf_write;
        int ch;

        buf_read = fopen(argv1, "r");
        buf_write = fopen(argv2, "a");

        while((ch = fgetc(buf_read)) != EOF)
       		fputc(ch, buf_write);

    	fclose(buf_write);
    	fclose(buf_read);
   }
   else
      printf("Il file: %s non può essere copiato\n", argv1);
}

void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf){

   struct dirent *entry;

   printf("A\n");
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
         //printf("file: %s\n",entry->d_name);


         strcat(percorsoCartella,entry->d_name);
         strcat(percorsoCartellaDest,entry->d_name);

         lstat(percorsoCartella,&statbuf1);
         
         //printf("%d\n", S_ISDIR(statbuf1.st_mode));
         dp1 = opendir(percorsoCartella);
         


         if(!S_ISDIR(statbuf1.st_mode)){

            CreaArchivio(percorsoCartellaDest, percorsoCartella);
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



void EstraiArchivio(){
	printf("Estrazione dell'archivio:\n");

}

void VisualizzaArchivio(){
	printf("Visualizzazione dell'archivio:\n");
}