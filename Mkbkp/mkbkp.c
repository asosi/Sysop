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
#define BUFSIZE 512
#define BUFSIZE1 1
#define NOT_EXIST -1


//***********************************HEADER***********************************
void CreaArchivio(char* percorsoDest, char* percorso);
void CreaArchivioCartelle(char* percorsoDest, char* percorso, int op);
void copia_file(char* percorso, char *from, char* to, int tofd);
void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf,int op);

void EstraiArchivio(char* percorso,char* percorsoDest);
void CreaPercorso(char* buf1);
void ScriviFile(int fromfd, char *from, char *to);
void CreateFolder(char *dirname);

void VisualizzaArchivio(char* argv1);
//****************************************************************************

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
  			//StampaBKP('0',percorsoDest,NULL);

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
                int op = open(percorsoDest, O_WRONLY | O_CREAT);
				copia_cartella(percorso,argv1,dp,statbuf,op);
                close(op);
    chmod(percorsoDest, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		    }
    	}
    	else if(x == 1) EstraiArchivio(percorsoDest,percorso);
    	else if(t == 1) VisualizzaArchivio(percorsoDest);
    }
}

//*******************************Creazione Archivio**********************************************

void CreaArchivio(char* percorsoDest, char* percorso){
	printf("Creazione dell'archivio:\n");

    char str[strlen(percorso)];
    strcpy(str,percorso);

    //char str[] = "now # is the time for all # good men to come to the # aid of their country";
    char delims[] = "/";
    char *result = NULL;
    char *nome = NULL;
    result = strtok( str, delims );
    while( result != NULL ) {
        printf( "result is \"%s\"\n", result );
        nome = result;
        result = strtok( NULL, delims );
    }

    printf("Il nome del file è: %s\n", nome);

    //Provo ad aprire il file di lettura
    int fromfd;  
    if ((fromfd = open(percorso, O_RDONLY)) == -1) 
    {
      perror(percorso);
      exit(EXIT_FAILURE);
    }


    int op = open(percorsoDest, O_WRONLY | O_CREAT);

    copia_file(nome,percorso,percorsoDest,op); //contenuto file

    close(op);

    chmod(percorsoDest, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
}

void CreaArchivioCartelle(char* percorsoDest, char* percorso, int op){
    printf("Creazione dell'archivio:\n");

    char str[strlen(percorso)];
    strcpy(str,percorso);

    //Provo ad aprire il file di lettura
    int fromfd;  
    if ((fromfd = open(percorso, O_RDONLY)) == -1) 
    {
      perror(percorso);
      exit(EXIT_FAILURE);
    }

    copia_file(percorso,percorso,percorsoDest,op);
}

void copia_file(char* percorso, char *from, char* to, int tofd) {
  int fromfd, nread;
  char buf[BUFSIZE];
  
  /* Apro i due file */
  if ((fromfd = open(from, O_RDONLY)) == -1) 
    {
      perror(from);
      exit(EXIT_FAILURE);
    }

  if (tofd == -1) 
    {
      perror(to);
      return;
    }
  write(tofd, percorso, strlen(percorso));
  write(tofd,"\n",1);
  /* Leggo da un file e scrivo sull'altro */
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
}

void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf,int op){

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

            //CreaArchivio(percorsoCartellaDest, percorsoCartella);
            CreaArchivioCartelle(percorsoCartellaDest, percorsoCartella,op);
         }
         else{
            strcat(percorsoCartella,"/");
            //strcat(percorsoCartellaDest,"/");

            //printf("Trovato cartella: %s\n",entry->d_name );
            copia_cartella(percorsoCartella,percorsoCartellaDest,dp1,statbuf1,op);
         }
      }
   }
   }
}
//*******************************Estrazione Archivio**********************************************

void EstraiArchivio(char* percorso,char* percorsoDest){
	printf("Estrazione dell'archivio:\n");

    int op = open(percorso,O_RDONLY);

    ScriviFile(op,percorso,percorsoDest);

    close(op);
}

void CreaPercorso(char* buf1){

    char* buf=(char*)malloc(sizeof(char)*500);;

    strcpy(buf,buf1);

    printf("%s\n",buf );

  
    int z;
    int ci=0;
    char* cartelle =(char*)malloc(sizeof(char)*500);
    for(z=0; z < strlen(buf)-1; z++){
        if(buf[z]=='/'){
            cartelle[ci] += buf[z];
            ci++;
            printf("cart: %s\n",cartelle);
            CreateFolder(cartelle);
        }
        else{
            cartelle[ci] += buf[z];
            ci++;
        }
    }
}

void ScriviFile(int fromfd, char *from, char *to){
    CreateFolder(to);
    chdir(to);
    int tofd, nread = 1;
    char buf[BUFSIZE1] = "\0";
    char percorso[500];
    int k=0;
    
    while(nread>0){
        while(buf[0] != '\n'){
            read(fromfd, buf, sizeof(buf));
            percorso[k]=buf[0];
            k++;
            //printf("%c\n",buf[0]);
        }

        percorso[k-1] = '\0';

        CreaPercorso(percorso);


        //CreateFolder(percorso);

        char fine[10] = "---fine--\n";
        int s = 0;
          /* Apro i due file */
        char supp[10];
        if (fromfd  == -1) 
        {
            perror(from);
            return;
        }

        tofd = open(percorso, O_WRONLY | O_CREAT );

        /* Leggo da un file e scrivo sull'altro */
        while (s!=10 && (nread = read(fromfd, buf, sizeof(buf)) > 0)){
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
        if (nread == -1)
            perror(from);
        /* Chiudo i file e  modifico i permessi */
        close(tofd);
        chmod(percorso, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
      
        nread = read(fromfd, buf, sizeof(buf));
        percorso[0] = buf[0];
        k = 1;
    }
}

void CreateFolder(char *dirname){

    //crea una nuova cartella con nome: dirname
   int check;
   check = mkdir(dirname,0777);
}

//*******************************Visualizza Archivio**********************************************

void VisualizzaArchivio(char* percorsoDest){

    int op = open(percorsoDest, O_RDONLY);
    
    int nread = 1;
    char buf[BUFSIZE1] = "\0";
    char percorso[500];
    int k=0;
    
    while(nread>0){
        while(buf[0] != '\n'){
            read(op, buf, sizeof(buf));
            percorso[k]=buf[0];
            k++;
            //printf("%c\n",buf[0]);
        }

        percorso[k-1] = '\0';

        printf("%s\n", percorso);


        char fine[10] = "---fine--\n";
        int s = 0;
          /* Apro i due file */
        char supp[10];

        /* Leggo da un file e scrivo sull'altro */
        while (s!=10 && (nread = read(op, buf, sizeof(buf)) > 0)){
            if(buf[0] != fine[s]){
                if(s>0){
                    int i=0;
                }
                s=0;
            }
            else{
                supp[s]=buf[0];
                s++;
            }
        }
        /* Chiudo i file e  modifico i permessi */
        nread = read(op, buf, sizeof(buf));
        percorso[0] = buf[0];
        k = 1;
    }
    close(op);
}
