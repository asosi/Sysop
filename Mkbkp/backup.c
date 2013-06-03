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
#define BUFSIZE 1


//***********************************HEADER***********************************
void CreaArchivio(char* percorsoDest, char* percorso);
void EstraiArchivio(char* percorso,char* percorsoDest);
void VisualizzaArchivio(char* argv1);

void copia_file(char* argv1, char* argv2);
void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf);
void StampaBKP(char k,char* percorsoDest, char*percorso1);

void LeggiFile(char* argv1,char* argv2);
void CreateFolder(char *dirname);

void copy(char *from, char *to) ;
void estrai(char *from, char *to);
//****************************************************************************


char* inizio = "--------------------------------------------------------------------inizio----------------------------------------------------------------\n";
char* fine = "--------------------------------------------------------------------fine--------------------------------------------------------------------\n";
char* riga = "\n--------------------------------------------------------------------------------------------------------------------------------------------\n";

int main(int argc, char *argv[]){

	char ch;
	char* percorsoDest = NULL;
	char* percorso = NULL;
	int c = 0, x = 0, t = 0;

    // controllo parametri passato all'eseguibile
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
            //Creazione archivio
  			StampaBKP('0',percorsoDest,NULL);

  			char* argv1= (char*)malloc(sizeof(char)*500);

	      	argv1 = strcpy(argv1,percorsoDest);

		    DIR *dp;
		    struct dirent *entry;
		    struct stat statbuf;
            dp = opendir(percorso);

            lstat(percorso,&statbuf);
		   
         	if(!S_ISDIR(statbuf.st_mode)){ //Se il file nn è una cartella, lo leggo e lo inserisco nell'archivio
    			CreaArchivio(percorsoDest,percorso);
		        return;
		    }
		    else{
                //il file è una cartella, controllo se c'è la / finale, altrimenti la inserisco
			   if(percorso[strlen(percorso)-1] != '/')
			        strcat(percorso,"/");

                //copio tutti i file della cartella
				copia_cartella(percorso,argv1,dp,statbuf);
		    }
    	}
    	else if(x == 1) EstraiArchivio(percorsoDest,percorso);
    	else if(t == 1) VisualizzaArchivio(percorsoDest);
    }

}

//*******************************Creazione Archivio**********************************************

void CreaArchivio(char* percorsoDest, char* percorso){
	printf("Creazione dell'archivio:\n");

    printf("%s\n", percorso);
    printf("%s\n", percorsoDest);

	//StampaBKP('i',percorsoDest,NULL); //inizio
	//StampaBKP('n',percorsoDest,percorso); //percorso

    //StampaBKP('-',percorsoDest,NULL); //riga vuota
	//StampaBKP('r',percorsoDest,NULL); //riga
	
	copia_file(percorso,percorsoDest); //contenuto file
    //copy(percorso,percorsoDest);

	//StampaBKP('-',percorsoDest,NULL); //riga vuota
	//StampaBKP('f',percorsoDest,NULL); //finee
}

void StampaBKP(char k,char* percorso, char*percorso1){
	FILE *buf_write;
	if(k!='0')
    	buf_write = fopen(percorso, "a");
    else
    	buf_write = fopen(percorso, "w");

    char ch;

    switch(k){
    	case '0': fputs("",buf_write); break;          //svuota il file
    	case 'i': fputs(inizio,buf_write); break;      //inserisco linea inizio file
    	case 'f': fputs(fine,buf_write); break;        //inserisco linea fine file
    	case 'r': fputs(riga,buf_write); break;        //inserisco riga di -
    	case 'n': fputs(percorso1,buf_write); break;   //inserisco percorso file
    	case '-': fputc('\n',buf_write);break;         //inserisco \n finale per ogni riga
      }
	fclose(buf_write);
}

void copia_file(char* argv1, char* argv2){   

	if(access(argv1,R_OK)==0){ //Controllo se ho i diritti di accesso al file

    	FILE *buf_read, *buf_write;
        int ch;

        buf_read = fopen(argv1, "rb");
        buf_write = fopen(argv2, "ab");

        //finchè nn ho finito di leggere il file, copio carattere per carattere in buf_write
        while((ch = fgetc(buf_read)) != EOF)
       		fputc(ch, buf_write);

    	fclose(buf_write);
    	fclose(buf_read);
   }
   else
      printf("Il file: %s non può essere copiato\n", argv1);
}


void copy(char *from, char *to) {
  int fromfd, tofd, nread;
  char buf[BUFSIZE];
  
  /* Apro i due file */

  if ((fromfd = open(from, O_RDONLY)) == -1) 
    {
      perror(from);
      return;
    }

  if ((tofd = open(to, O_WRONLY |O_APPEND | O_CREAT | O_TRUNC)) == -1) 
    {
      perror(to);
      return;
    }


  write(tofd, from, strlen(to));
  write(tofd,"\n",1);

  /* Leggo da un file e scrivo sull'altro */
    printf("xxxxxxx\n");
  while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
    if ((write(tofd, buf, nread)) != nread)
    perror(to);

  if (nread == -1)
    perror(from);
  char fine[10] = "---fine--\n";
  /* Chiudo i file e  modifico i permessi */
  write(tofd, fine, 10);
  if(close(fromfd) == -1)
    perror(from);
  if(close(tofd) == -1)
    perror(to);
  chmod(to, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf){

   struct dirent *entry;

   printf("A\n");
   while((entry = readdir(dp)) != NULL){


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
        // strcat(percorsoCartellaDest,entry->d_name);

         lstat(percorsoCartella,&statbuf1);
         
         //printf("%d\n", S_ISDIR(statbuf1.st_mode));
         dp1 = opendir(percorsoCartella);
         


         if(!S_ISDIR(statbuf1.st_mode)){

            CreaArchivio(percorsoCartellaDest, percorsoCartella);
         }
         else{
            strcat(percorsoCartella,"/");
            //strcat(percorsoCartellaDest,"/");

            //printf("Trovato cartella: %s\n",entry->d_name );
            copia_cartella(percorsoCartella,percorsoCartellaDest,dp1,statbuf1);
         }
      }
   }
   }
}
//*******************************Estrazione Archivio**********************************************

void EstraiArchivio(char* percorso,char* percorsoDest){
	printf("Estrazione dell'archivio:\n");
    estrai(percorso,percorsoDest);
}
/*
void LeggiFile(char* argv1,char* argv2){

    //legge il file .bkp ed estrae tutti i file memorizzati

    FILE *buf_read, *buf_write;
    char buf[1000];
    char* ch;
    int k = 0;

    buf_read = fopen(argv1, "rb");

    printf("%s\n",argv2 );

    while(1) {
        ch=fgets(buf, 1000, buf_read);
        if( ch==NULL )
          break;
        //printf("%s\n", buf);
        if(strcmp(buf,inizio)==0)
            k = 1;
        else{
            if(strcmp(buf,fine)!=0 && strcmp(buf,riga)!=0){
                if(k == 1){
                    k = 0;

                    char* percorso=(char*)malloc(sizeof(char)*500);;
                    strcat(percorso,argv2);
                    strcat(percorso,buf);
                    percorso[strlen(percorso)-1] = 0;

                   // printf("percorso: %s\n",percorso );

                    int z;
                    int ci=0;
                    char* cartelle =(char*)malloc(sizeof(char)*500);
                    for(z=0; z < strlen(percorso)-1; z++){
                        if(percorso[z]=='/'){
                            cartelle[ci] += percorso[z];
                            ci++;
                            CreateFolder(cartelle);
                            //printf("%s\n", cartelle);
                            
                        }
                        else{
                            cartelle[ci] += percorso[z];
                            ci++;
                        }
                    }


                    buf_write = fopen(percorso, "ab");
                }
                else{                
                    fputs(buf, buf_write);
                }
            }
        }
    }


    fclose(buf_write);
    fclose(buf_read);
}
*/

void estrai(char *from, char *to) {
  int fromfd, tofd, nread;
  char buf[BUFSIZE];
  char fine[10] = "---fine---";
  int s = 0;
  /* Apro i due file */
  char supp[10];
  if ((fromfd = open(from, O_RDONLY)) == -1) 
    {
      perror(from);
      return;
    }


int g = 0;
int ind=0;
char* per=(char*)malloc(sizeof(char)*500);

  /* Leggo da un file e scrivo sull'altro */
    printf("xxxxxxx\n");
  while ((nread = read(fromfd, buf, sizeof(buf))) > 0 && s!=10){
    if(g==0)
        if(buf[0]!='\n')
            per[ind] = buf[0];
        else{
            g++;
            char* percorso=(char*)malloc(sizeof(char)*500);;
            strcat(percorso,to);
            strcat(percorso,buf);
            percorso[strlen(percorso)-1] = 0;

           // printf("percorso: %s\n",percorso );

            int z;
            int ci=0;
            char* cartelle =(char*)malloc(sizeof(char)*500);
            for(z=0; z < strlen(percorso)-1; z++){
                if(percorso[z]=='/'){
                    cartelle[ci] += percorso[z];
                    ci++;
                    CreateFolder(cartelle);
                    
                }
                else{
                    cartelle[ci] += percorso[z];
                    ci++;
                }
            }

            if ((tofd = open(percorso, O_WRONLY | O_CREAT | O_TRUNC)) == -1) 
            {
              perror(to);
              return;
            }
        }

    if(buf[0] != fine[s]){
      if(s>0){
        int i=0;
        for(i=0; i<s; i++)
          write(tofd, supp, nread);  
      }
      write(tofd, buf, nread);
      s=0;
    }
    else{
      supp[s]=buf[0];
      s++;
    }
  }
  if(nread = read(fromfd, buf, sizeof(buf))!=0 && s==10){
    printf("sono uscito per sbaglio\n");
  }
  if (nread == -1)
    perror(from);
  /* Chiudo i file e  modifico i permessi */
  if(close(fromfd) == -1)
    perror(from);
  if(close(tofd) == -1)
    perror(to);
  chmod(to, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

void CreateFolder(char *dirname){

    //crea una nuova cartella con nome: dirname
   int check;
   check = mkdir(dirname,0777);

   if(!check)
   printf("Creata cartella: %s\n", dirname);
}

//*******************************Visualizza Archivio**********************************************

void VisualizzaArchivio(char* argv1){
	printf("Visualizzazione dell'archivio, i file presenti sono:\n");

    FILE *buf_read;
    char buf[1000];
    char* ch;
    int k = 0;

    buf_read = fopen(argv1, "r");

    while(1) {
        ch=fgets(buf, 1000, buf_read);
        if( ch==NULL )
            break;
         if(strcmp(buf,inizio)==0)
            k = 1;
        else{
            if(strcmp(buf,fine)!=0 && strcmp(buf,riga)!=0){
                if(k == 1){         
                    char* stampa = (char*)malloc(sizeof(char)*500); 
                    buf[strlen(buf)-1] = 0;

                    strcpy(stampa,buf);
                    int fine = 0;

                    while(fine != 1){
                        if(stampa[0] != '.'){
                            fine = 1;                            
                            printf("%s\n",stampa);
                        }
                        if(stampa[0] = '.' && stampa[1] == '.' && stampa[2] == '/')
                            strcpy(stampa,stampa+3);
                    }
                    free(stampa);
                    k = 0;
                }
            }
        }
    }
}
