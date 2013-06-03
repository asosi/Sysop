#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <ncurses.h>
#include "makelog.h" //includo per generare file di log

//stringa che mi contiene 
char argomento[20];

//struct per contenere dati processo
struct datiproc{
    char name[1024];
    char pid[1024];
    char ppid[1024];
    char utime[1024];
    char stime[1024];
    double percentuale;
};

void infoproc(char *path);
double calcpu(char *percorso);
void percpu(int nproc);
void merge(int start, int center, int end);
void Mergesort(int start, int end);
void stampaProcessi(int n);

//numero processi
int nproc = 0;
//numero di processi da stampare
int numerop=0;

struct datiproc** elenco;


void movedir(char *dir){

    elenco = malloc(sizeof(struct datiproc*));
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    //controllo se riesco ad aprire la directory, se non riesco esco e scrivo in log
    if((dp = opendir(dir)) == NULL) {
        printf("%s\n", "Attenzione! Impossibile aprire la directory /proc/");
        writeERROR(argomento, "Attenzione! Impossibile aprire la directory /proc/");
        exit(EXIT_FAILURE); 
    }

    chdir(dir);

    //ciclo ogni elemento della directory
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);

        // controllo se ci sono elementi che iniziano con lettere o elementi .<nomefile> (nascosti) e li ignoro
        if(entry->d_name[0] == '.' || entry->d_name[0] == 'a' || entry->d_name[0] == 'b' || entry->d_name[0] == 'c' || entry->d_name[0] == 'd' || entry->d_name[0] == 'e' || entry->d_name[0] == 'f' || entry->d_name[0] == 'g' || entry->d_name[0] == 'h' || entry->d_name[0] == 'i' || entry->d_name[0] == 'j' || entry->d_name[0] == 'k' || entry->d_name[0] == 'l' || entry->d_name[0] == 'm' || entry->d_name[0] == 'n' || entry->d_name[0] == 'o' || entry->d_name[0] == 'p' || entry->d_name[0] == 'q' || entry->d_name[0] == 'r' || entry->d_name[0] == 's' || entry->d_name[0] == 't' || entry->d_name[0] == 'u' || entry->d_name[0] == 'v' || entry->d_name[0] == 'w' || entry->d_name[0] == 'x' || entry->d_name[0] == 'y' || entry->d_name[0] == 'z'
             || entry->d_name[0] == 'A'
             || entry->d_name[0] == 'B'
             || entry->d_name[0] == 'C'
             || entry->d_name[0] == 'D'
             || entry->d_name[0] == 'E'
             || entry->d_name[0] == 'F'
             || entry->d_name[0] == 'G'
             || entry->d_name[0] == 'H'
             || entry->d_name[0] == 'I'
             || entry->d_name[0] == 'J'
             || entry->d_name[0] == 'K'
             || entry->d_name[0] == 'L'
             || entry->d_name[0] == 'M'
             || entry->d_name[0] == 'N'
             || entry->d_name[0] == 'O'
             || entry->d_name[0] == 'P'
             || entry->d_name[0] == 'Q'
             || entry->d_name[0] == 'R'
             || entry->d_name[0] == 'S'
             || entry->d_name[0] == 'T'
             || entry->d_name[0] == 'U'
             || entry->d_name[0] == 'V'
             || entry->d_name[0] == 'W'
             || entry->d_name[0] == 'X'
             || entry->d_name[0] == 'Y'
             || entry->d_name[0] == 'Z'){
                continue;
            }
        //controllo se è una cartella
        if(S_ISDIR(statbuf.st_mode)) {
            // Cerca una directory e ignora . e .. e se li trovo riparto con il ciclo
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || entry->d_name[0] == '.'){
                continue;
            }
            
            //concateno la stringa della dir con la cartella per creare il path di ogni cartella dei proc
            char new_path[256];
            new_path[0] = '\0';  //stringa vuota
            strcat(new_path,dir);
            strcat(new_path,entry->d_name);
            strcat(new_path,"/");

            //chiamo la funzione che entra nella sottodirectory del processo
            infoproc(new_path);
        }
    }
    closedir(dp);
}


void infoproc(char *path){

    FILE *gostatus;
    char parola[1024];
    DIR *des;
    struct dirent *entry;
    struct stat statbuf;

    //controllo se riesco ad aprire la directory, se non riesco esco e scrivo in log
    if((des = opendir(path)) == NULL) {
        printf("Attenzione! Impossibile aprire la directory %s\n", path);
        writeERROR(argomento, "Attenzione! Impossibile aprire la directory /proc/<PID>");
        exit(EXIT_FAILURE);
    }

    chdir(path);

    //ciclo ogni elemento della directory
    while((entry = readdir(des)) != NULL) {
        lstat(entry->d_name,&statbuf);

        //ignoro i file . e .. e quelli .<nomefile>
        if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || entry->d_name[0] == '.'){
            continue;
        }
        
        //se ci sono cartelle contenute nella directory le ignoro
        if(S_ISDIR(statbuf.st_mode)){
            continue;
        }else{

            //controllo se nella directory in cui mi trovo esiste un file chiamato "status"
            if(strcmp("stat",entry->d_name) == 0){

                elenco = realloc(elenco,(sizeof(struct datiproc*)*(nproc+1))); 
                elenco[nproc] = malloc(sizeof(struct datiproc));

                // apro il file stat in lettura
                gostatus = fopen("stat", "r");

                //controllo se riesco ad aprire il file stat contenuto in /proc/<PID>/stat
                if(!gostatus){
                    printf("Attenzione! Impossibile aprire il file /proc/%s/%s\n", entry->d_name, "stat");
                    writeERROR(argomento, "Attenzione! Impossibile aprire il file /proc/<PID>/stat");
                    exit(EXIT_FAILURE);
                }

                //File /proc/<PID>/stat contiene:
                //Posizione 1 : PID
                //Posizione 2 : Nome dell' eseguibile del processo
                //Posizione 4 : PPID
                //Posizione 14 : utime
                //Posizione 15 : stime

                fscanf(gostatus,"%s", parola);
                strcpy(elenco[nproc]->pid, parola);

                fscanf(gostatus,"%s", parola);
                strcpy(elenco[nproc]->name, parola);

                fscanf(gostatus,"%s", parola);

                fscanf(gostatus,"%s", parola);
                strcpy(elenco[nproc]->ppid, parola);

                int p;
                for(p=0;p<9;p++){
                    fscanf(gostatus,"%s", parola);
                }

                fscanf(gostatus,"%s", parola);
                strcpy(elenco[nproc]->utime, parola);

                fscanf(gostatus,"%s", parola);
                strcpy(elenco[nproc]->stime, parola);

                percpu(nproc);
                nproc++;

                fclose(gostatus);
            }
        }
    }
    closedir(des);
}



// metodo calcolo CPU total
double calcpu(char *percorso){


    FILE *gostat;
    char parola[1024];
    double totalCPU = 0.0;
    double temp = 0.0;
    DIR *dir2;

    struct dirent *entry;
    struct stat statbuf;

    //controllo se riesco ad aprire la directory
    if((dir2 = opendir(percorso)) == NULL) {
        printf("Attenzione! Impossibile aprire la directory /proc/\n");
        writeERROR(argomento, "Attenzione! Impossibile aprire la directory /proc/");
        exit(EXIT_FAILURE);
    }

    chdir(percorso);

    //ciclo ogni elemento della directory
    while((entry = readdir(dir2)) != NULL) {
        lstat(entry->d_name,&statbuf);
        
        //se ci sono cartelle contenute nella directory le ignoro
        if(S_ISDIR(statbuf.st_mode)){
            continue;
        }else{

            //controllo se nella directory in cui mi trovo esiste un file chiamato "stat"
            if(strcmp("stat",entry->d_name) == 0){

                //apro il file stat in lettura
                gostat = fopen("stat", "r");

                //controllo se riesco ad aprire il file stat in /proc/
                if(!gostat){
                    printf("Attenzione! Impossibile aprire il file /proc/stat\n");
                    writeERROR(argomento, "Attenzione! Impossibile aprire il file /proc/stat");
                    exit(EXIT_FAILURE);
                }

                //fscanf mi prende una parola alla volta del file
                while(!feof(gostat)){

                    fscanf(gostat,"%s", parola);
 
                    if(strcmp("cpu", parola) == 0){
                        continue;
                    }
                    if(strcmp("cpu0", parola) == 0){
                        closedir(dir2);
                        fclose(gostat);
                        return totalCPU;
                    }
                    temp = atof(parola);
                    totalCPU += temp;               
                }
            }
        }
    }
}



//funzione per calcolare la % della cpu usata da un processo
void percpu(int nproc){

    double per = 0.0;
    double cpu_total = 0.0;
    double pcpu = 0.0;
    double utime = 0.0;
    double stime = 0.0;

    //calcolo CPU total del sistema e valori utime e stime del processo    
    utime = atof(elenco[nproc]->utime);
    stime = atof(elenco[nproc]->stime);
    cpu_total = calcpu("/proc/");

    //calcolo CPU processo con utime e stime
    pcpu = utime + stime;

    //calcolo %CPU processo
    per = (pcpu / cpu_total)*100.0;

    //inserimento valore nel campo percentuale della struct
    elenco[nproc]->percentuale = per;
}



int main(int argc, char *argv[]){

    char ch;
    int n = 0;
    char* valore;
    int numerop=0;
    char car; //carattere preso dalla getch
    int timewait = 1;
	strcpy(argomento,argv[0]);

    //se lancio solo l'eseguibile (argc=1) allora passo alla initlog il valore NULL
	if(argc == 1){
		initlog(argomento, NULL,NULL,NULL);
	}else{
		initlog(argomento, argv[2],NULL,NULL);
	}
    //getopt per passare il parametro -n <numero> , ovvero per mostrare "numero" processi
    while((ch = getopt (argc, argv, "n: ")) != -1){
        switch (ch){
                case 'n': n=1; valore = optarg;  break;
                default: numerop=10;    
        }
    }
	//controllo se sono inseriti piu di 3 parametri
	if(argc > 3){
		printf("%s\n","Troppi argomenti! <eseguibile> -n <num>");
        	writeERROR(argomento, "Attenzione! Troppi parametri passati");
        	exit(EXIT_FAILURE); 
	}
    //caso default stampa 10 processi
    if(argv[1] == NULL){
        n++;
        valore="10";
    }
    //controllo se il valore non è inserito
    if(n==0){
        printf("%s\n","Valore -n non passato! <eseguibile> -n <num>");
        writeERROR(argomento, "Attenzione! Inserimento sbagliato o valore -n <valore> non passato");
        exit(EXIT_FAILURE);   
    }else{
        //cast della stringa in intero
        numerop = atoi(valore);
    	
    	//mi serve per uscire se come parametro dopo -n mi passa dei caratteri
    	if(numerop == 0){
    		printf("%s\n","Passare un numero da 1 a 40 dopo -n");
           	writeERROR(argomento, "Attenzione! Passare un numero da 1 a 40 dopo -n");
            exit(EXIT_FAILURE); 
    	}
        //controllo che il numero n di processi da stampare non sia > 40 o < 0
    	if(numerop > 40 || numerop < 0){
            printf("%s\n", "Attenzione! Si possono stampare al massimo 40 processi!");
            writeERROR(argomento, "Attenzione! Si possono stampare al massimo 40 processi");
    	    exit(EXIT_FAILURE);	
    	}else{
            //creo stringa da inserire nel file log
            char output[256];
	        strcat(output,"Ho stampato a video ");
	        strcat(output, valore);
		    strcat(output," processi");
		    writeOUTPUT(argomento, output);
	    }
        do{
            initscr();
            printw("######################################################\n"); 
            printw("#%8s %8s   %8s   %20s #\n", "<PID>","<PPID>","<%CPU>","<Name>");
            printw("######################################################\n"); 
            timeout(timewait*1000);
            scrollok(stdscr,TRUE);
            movedir("/proc/");
       	    stampaProcessi(numerop);
		    printw("######################################################\n");
		    printw("Premere 'q' o 'Q' per uscire.\n");
            nproc=0;
            car = getch();
            if(car>48 && car<58){
                timewait = car-48;
            }
            clear();
            refresh();
        }
        while( car != 'q' && car != 'Q');
        endwin();
        return 0;
    }
}


void stampaProcessi(int n){
    int i;
    Mergesort(0,nproc-1);
    for(i=0; i<n; i++){
        printw("#%8s|%8s|%10f %%|%20s #\n",elenco[i]->pid, elenco[i]->ppid, elenco[i]->percentuale, elenco[i]->name);
    }
}

void merge(int start, int center, int end){
  int s = start;
  int c = center+1;
  struct datiproc* supp[end-start];
  int i = 0;
  while(s<=center && c<=end){
    if(elenco[s]->percentuale > elenco[c]->percentuale){
      supp[i] = elenco[s];
      s++;
    }
    else{
      supp[i] = elenco[c];
      c++;
    }
    i++;
  }
  while(s<=center){
    supp[i] = elenco[s];
    s++;
    i++;
  }
  while(c<=end){
    supp[i] = elenco[c];
    c++;
    i++;
  }
  for(i=start; i<=end; i++)
    elenco[i] = supp[i-start];
}

void Mergesort(int start, int end){
  if(start<end){
    int center = (start+end)/2;
    Mergesort(start,center);
    Mergesort(center+1,end);
    merge(start,center,end);
  }
}
