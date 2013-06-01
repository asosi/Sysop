#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <ncurses.h>

//struct per contenere dati processo
struct datiproc{
    char name[256];
    char pid[100];
    char ppid[100];
    char utime[100];
    char stime[100];
    double percentuale;
};

void infoproc(char *path);
double calcpu(char *percorso);
void percpu(int nproc);

//numero processi
int nproc = 0;


struct datiproc** elenco;


void movedir(char *dir){

    elenco = malloc(sizeof(struct datiproc*));

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        perror(dir);
        return;
    }

    chdir(dir);

    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);

        // SO CHE é UNA CHECCA COME COSA!
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

        if(S_ISDIR(statbuf.st_mode)) {
            // Cerca una directory e ignora . e .. e se li trovo riparto con il ciclo
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || entry->d_name[0] == '.'){
                continue;
            }
            
            //concateno la stringa della dir con la cartella per creare il path di ogni cartella dei proc
            char new_path[256];
            new_path[0] = '\0';  // ensures the memory is an empty string
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
    if((des = opendir(path)) == NULL) {
        perror(path);
        return;
    }

    chdir(path);

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

                //############################ ENTRO FILE STAT ############################
                // apro il file status in lettura
                gostatus = fopen("stat", "r");


                if(!gostatus){
                    printf("%s\n", "Errore apertura file 'proc/%s/stat'!",entry->d_name);
                }

                int n = 0;

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

                printw("|%8s|%8s|%10f %%|%20s |\n", elenco[nproc]->pid, elenco[nproc]->ppid, elenco[nproc]->percentuale, elenco[nproc]->name);
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

    //variabile per calcolo CPU total
    double totalCPU;

    double temp = 0.0;

    DIR *dir2;

    struct dirent *entry;
    struct stat statbuf;
    if((dir2 = opendir(percorso)) == NULL) {
        perror(percorso);
        return;
    }

    chdir(percorso);

    while((entry = readdir(dir2)) != NULL) {
        lstat(entry->d_name,&statbuf);
        
        //se ci sono cartelle contenute nella directory le ignoro
        if(S_ISDIR(statbuf.st_mode)){
            continue;
        }else{

            //controllo se nella directory in cui mi trovo esiste un file chiamato "stat"
            if(strcmp("stat",entry->d_name) == 0){

                //############################ ENTRO FILE STAT ############################
                // apro il file stat in lettura
                gostat = fopen("stat", "r");

                if(!gostat){
                    printf("%s\n", "Errore apertura file 'proc/stat'!");
                }

                // fscanf mi prende una parola alla volta del file
                while(!feof(gostat)){

                    fscanf(gostat,"%s", parola);
 
                    if(strcmp("cpu", parola) == 0){
                        continue;
                    }
                      if(strcmp("cpu0", parola) == 0){
                        return totalCPU;
                    }

                    temp = atof(parola);
                    totalCPU += temp;
                    
                }
                return totalCPU;
                
                fclose(gostat);

            }
        }
    }

    closedir(dir2);
}



// funzione per calcolare la % della cpu usata da un processo
void percpu(int nproc){

    double per = 0.0;
    double cpu_total = 0.0;

    double pcpu = 0.0;
    double utime = 0.0;
    double stime = 0.0;

    //calcolo CPU total del sistema e valori utime e stime del processo    
    utime = atof(elenco[nproc]->utime);
    stime = atof(elenco[nproc]->stime);
    cpu_total = calcpu("/Users/Andrea/Desktop/");

    //calcolo CPU processo con utime e stime
    pcpu = utime + stime;

    per = (pcpu / cpu_total)*100.0;

    //calcolo percentuale finale
    elenco[nproc]->percentuale = per;
}



int main(int argc, char *argv[]){

    char ch;
    int n = 0;
    char* valore;
    int numerop=0;
    

    //carattere preso dalla getch
    char car;

    //getopt per passare il parametro -n <numero> , ovvero per mostrare "numero" processi
    while((ch = getopt (argc, argv, "n: ")) != -1){
        switch (ch){
                case 'n': n=1; valore = optarg;  break;
                default: numerop=10;    
        }
    }

    // caso default stampa 10 processi
    if(argv[1] == NULL){
        n++;
        valore="10";
    }

    if(n==0){
        printf("%s\n","Valore -n non passato! <eseguibile> -n <num>");    
    }else{
        //cast della stringa in intero
        numerop = atoi(valore);
        do{
            initscr();
            printw("Premere 'q' o 'Q' per uscire.\n");
            printw("####################################################\n"); 
            printw(" %8s %8s   %8s %20s  \n", "<PID>","<PPID>","<CPU>","<Name>");
            movedir("/Users/Andrea/Desktop/proc/");
            car = getch();
            clear();
            refresh();
        }
        while( car != 'q' && car != 'Q');
        endwin();
        return 0;
    }
}


