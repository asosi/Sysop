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
#include "makelog.h"
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

char argomento[20];


int main(int argc, char *argv[]){

	char ch;
	char* percorsoDest = NULL;
	char* percorso = NULL;
	int c = 0, x = 0, t = 0;

    strcpy(argomento,argv[0]);

    // controllo parametri passato all'eseguibile
	while ((ch = getopt (argc, argv, "f: cxt")) != -1){
        switch (ch){
        	case 'f': percorsoDest = optarg; percorso = argv[optind];  break; //-f <archivio> indica l'archivio da creare od estrarre. 
        	case 'c': c = 1; break; 			//-c indica la creazione dell'archivio
        	case 'x': x = 1; break; 			//-x indica l'estrazione dell'archivio nella directory corrente
        	case 't': t = 1; break; 			//-t indica che deve essere visualizzato un elenco del contenuto dell'archivio
        }
    }

    //Controllo i parametri inseriti
    if(percorso == NULL && c == 1){
        writeERROR(argomento,"Non hai inserito l'archivio da creare");
        printf("Non hai inserito l'archivio da creare\n");        
        exit(EXIT_FAILURE);
    }
    else if(percorso == NULL && x == 1){
        writeERROR(argomento,"Non hai inserito l'archivio da estrarre");
    	printf("Non hai inserito l'archivio da estrarre\n"); 
        exit(EXIT_FAILURE);
    }   
    else if(percorso == NULL && t == 1){
        writeERROR(argomento,"Non hai inserito l'archivio da visualizzare");
    	printf("Non hai inserito l'archivio da visualizzare\n");
        exit(EXIT_FAILURE);
    }
    else{
        //Creo il file .bkp
    	if(c == 1){ 
            //Controllo se esiste un gia il file .bkp
            int first = open(percorsoDest,O_RDONLY);
            if(first!=NOT_EXIST){
                writeERROR(argomento,"Il file di destinazione .bkp esiste gia!!!");
                printf("Il file di destinazione .bkp esiste gia!!!\n");
                exit(EXIT_FAILURE);
            }

            //Controllo se la cartella/file da copiare esiste
            int second = open(percorso,O_RDONLY);
            if(second==-1){
                writeERROR(argomento,"Il file/directory da copiare non esiste!!!");
                printf("Il file/directory da copiare non esiste!!!\n");
                exit(EXIT_FAILURE);
            }

            printf("Creazione dell'archivio!\n");
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
    	else if(x == 1){ // Estraggo il file .bkp
            int first = open(percorsoDest,O_RDONLY);
            if(first==-1){
                writeERROR(argomento,"Il file da estrarre (.bkp) non esite!!!");
                printf("Il file da estrarre (.bkp) non esite!!!\n");
                exit(EXIT_FAILURE);
            }
            EstraiArchivio(percorsoDest,percorso);
        }
    	else if(t == 1){ //Visualizzo a video il file .bkp
            int first = open(percorsoDest,O_RDONLY);
            if(first==-1){
                writeERROR(argomento,"Il file da visualizzare (.bkp) non esite!!!");
                printf("Il file da visualizzare (.bkp) non esite!!!\n");
                exit(EXIT_FAILURE);                
            }
            VisualizzaArchivio(percorsoDest);
        }
    }
}

//*******************************Creazione Archivio**********************************************
//Crea l'archivio di un singolo file
void CreaArchivio(char* percorsoDest, char* percorso){

    char str[strlen(percorso)];
    strcpy(str,percorso);

    char delims[] = "/";
    char *result = NULL;
    char *nome = NULL;
    result = strtok( str, delims );
    while( result != NULL ) {
        nome = result;
        result = strtok( NULL, delims );
    }

    //Provo ad aprire il file di lettura
    int fromfd;  
    if ((fromfd = open(percorso, O_RDONLY)) == -1) 
    {
        writeERROR(argomento,"Impossibile aprire il file di lettura");                
        perror(percorso);
        exit(EXIT_FAILURE);
    }


    int op = open(percorsoDest, O_WRONLY | O_CREAT);   //apro il file .bkp
    copia_file(nome,percorso,percorsoDest,op); //Copio il contenuto del file, nel file .bkp
    close(op);  //chiudo il file .bkp
    chmod(percorsoDest, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //Aggiungo permessi al file .bkp
}

//Crea l'archivio di una cartella
void CreaArchivioCartelle(char* percorsoDest, char* percorso, int op){

    char str[strlen(percorso)];
    strcpy(str,percorso);

    //Provo ad aprire il file di lettura
    int fromfd;  
    if ((fromfd = open(percorso, O_RDONLY)) == -1) 
    {
        writeERROR(argomento,"Impossibile aprire il file di lettura");
      perror(percorso);
      exit(EXIT_FAILURE);
    }

    copia_file(percorso,percorso,percorsoDest,op);
}

//Metodo che inserisce un file nel file .bkp
void copia_file(char* percorso, char *from, char* to, int tofd) {
  int fromfd, nread;
  char buf[BUFSIZE];
  
  /* Apro i due file */
  if ((fromfd = open(from, O_RDONLY)) == -1) 
    {
        writeERROR(argomento,"Impossibile aprire il file di lettura");
      perror(from);
      exit(EXIT_FAILURE);
    }

  if (tofd == -1) 
    {
        writeERROR(argomento,"Impossibile aprire il file di scrittura");
      perror(to);
      return;
    }

    //Scrivo per prima cosa il percorso del file
    write(tofd, percorso, strlen(percorso));

    //Scrivo uno \n
    write(tofd,"\n",1);

    /* Leggo da un file e scrivo sull'altro */
    while ((nread = read(fromfd, buf, sizeof(buf))) > 0)
        if ((write(tofd, buf, nread)) != nread)
            perror(to);

    if (nread == -1)
        perror(from);
    char fine[10] = "---fine--\n";
    
    //Scrivo una stringa di fine file
    write(tofd, fine, 10);

    //Chiudo il file
    if(close(fromfd) == -1)
        perror(from);
}

//Metodo che inserisce nel file .bkp tutti i file di una  cartella
void copia_cartella(char* read, char* write, DIR *dp, struct stat statbuf,int op){

   struct dirent *entry;

   //Ciclo su tutti i file contenuti nella cartella
   while((entry = readdir(dp)) != NULL){

    //Controllo se ho i diritti di accesso al file
    if(access(read,R_OK)==0){

      DIR *dp1;
      struct stat statbuf1;


      if(strcmp(".",entry->d_name)!=0 && strcmp("..",entry->d_name)!=0 && entry->d_name[0] !='.')
      {

         char* percorsoCartella = (char*)malloc(sizeof(char)*500);
         char* percorsoCartellaDest = (char*)malloc(sizeof(char)*500);

         percorsoCartella = strcpy(percorsoCartella,read);
         percorsoCartellaDest = strcpy(percorsoCartellaDest,write);

         strcat(percorsoCartella,entry->d_name);

         lstat(percorsoCartella,&statbuf1);
         
         dp1 = opendir(percorsoCartella);   

         //Se il file trovato non è una cartella, lo inserisco nel file .bkp
         if(!S_ISDIR(statbuf1.st_mode)){
            CreaArchivioCartelle(percorsoCartellaDest, percorsoCartella,op);
         }
         else{
            //Aggiungo una / al percorso e richiamo il metodo per le cartelle
            strcat(percorsoCartella,"/");
            copia_cartella(percorsoCartella,percorsoCartellaDest,dp1,statbuf1,op);
         }
      }
   }
   }
}


//*******************************Estrazione Archivio**********************************************
//Metodo che estrae dal file .bkp
void EstraiArchivio(char* percorso,char* percorsoDest){
	printf("Estrazione dell'archivio!\n");

    int op = open(percorso,O_RDONLY); //Apro il file di lettura

    ScriviFile(op,percorso,percorsoDest); //Copio il file 

    close(op); //Chiudo il file di lettura
}

//Metodo che analizza il percorso inserito nel file .bkp, e creo tutte le sottocartelle
void CreaPercorso(char* buf1){
    char* buf=(char*)malloc(sizeof(char)*500);
    strcpy(buf,buf1);  
    int z;
    int ci=0;
    char* cartelle =(char*)malloc(sizeof(char)*500);
    for(z=0; z < strlen(buf)-1; z++){
        if(buf[z]=='/'){
            cartelle[ci] += buf[z];
            ci++;
            CreateFolder(cartelle);
        }
        else{
            cartelle[ci] += buf[z];
            ci++;
        }
    }
}

//Metodo che legge il file .bkp e crea i file/directory lette
void ScriviFile(int fromfd, char *from, char *to){
    //Creo la cartella di destinazione passata dall'utente come parametro e ci entro
    CreateFolder(to);
    chdir(to);

    int tofd, nread = 1;
    char buf[BUFSIZE1] = "\0";
    char percorso[500];
    int k=0;
    
    //Finchè non ho finito di leggere il file ciclo
    while(nread>0){

        //Leggo il percorso di destinazione dal file
        while(buf[0] != '\n'){
            read(fromfd, buf, sizeof(buf));
            percorso[k]=buf[0];
            k++;
        }

        //metto carattere di fine stringa al percorso
        percorso[k-1] = '\0';

        //Creo tutte le cartelle e sottocartelle che ci sono nel percorso letto
        CreaPercorso(percorso);

        //Stringa di fine file
        char fine[10] = "---fine--\n";
        int s = 0;

        //Apro il file di scrittura
        char supp[10];
        if (fromfd  == -1) 
        {
            perror(from);
            return;
        }

        tofd = open(percorso, O_WRONLY | O_CREAT );

        //Leggo da un file e scrivo sull'altro finchè non trovo la stringa di fine file
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

//Metodo che crea una cartella
void CreateFolder(char *dirname){

    //crea una nuova cartella con nome: dirname
   int check;
   check = mkdir(dirname,0777);
}


//*******************************Visualizza Archivio**********************************************
//Metodo che visualizza a video tutti i file contenuti nell'archivio (.bkp)
void VisualizzaArchivio(char* percorsoDest){
    printf("Visualizzazione dell'archivio: I file presenti nel file %s sono:\n",percorsoDest);

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
