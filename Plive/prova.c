#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <ncurses.h>

void infoproc(char *path);


void movedir(char *dir){

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

            // stampa le directory contenute nel path dato, aggiunge "/" cosi si identificano dai file
            //printf("%s/\n",entry->d_name);

            
            //concateno la stringa della dir con la cartella per creare il path di ogni cartella dei proc
            char new_path[256];
            new_path[0] = '\0';  // ensures the memory is an empty string
            strcat(new_path,dir);
            strcat(new_path,entry->d_name);
            strcat(new_path,"/");

            //chiamo la funzione che entra nella sottodirectory del processo
            infoproc(new_path);

            //chiamo la funzione calcpu
            calcpu("/proc/");
        }
        // stampa i file contenuti nel path dato
        //else printf("%s\n",entry->d_name);
    }
    closedir(dp);
}


void infoproc(char *path){


    FILE *gostatus;
    char parola[1024];

    // numero dei processi per stamparli
    int nproc = 0;

    //struct per contenere dati processo
    struct datiproc{
      char state[10];
      char name[256];
      char pid[100];
      char ppid[100];
      char vmrss[1000];
      char vmsize[1000];
    };

    DIR *des;
    struct datiproc* pro;
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
            //stampo ogni elemento contenuto nella directory che non sia una directory
            //printf("\t%s\n",entry->d_name);

            //controllo se nella directory in cui mi trovo esiste un file chiamato "status"
            if(strcmp("status",entry->d_name) == 0){

                //############################ ENTRO FILE STATUS ############################
                // apro il file status in lettura
                gostatus = fopen("status", "r");

                //inizializzo pro
                pro = malloc(sizeof(struct datiproc));

                //inizializzo array "elenco" di struct
                struct datiproc** elenco = malloc(sizeof(struct datiproc*));

                if(!gostatus){
                    printf("%s\n", "Errore apertura file 'proc/%s/status'!",entry->d_name);
                }

                nproc++;
                int n = 0;
                // fscanf mi prende una parola alla volta del file
                while(!feof(gostatus)){

                    fscanf(gostatus,"%s", parola);

                    if(strcmp("Pid:", parola) == 0){
                        fscanf(gostatus,"%s", parola);
                        strcpy(pro->pid, parola);

                    }
                    if(strcmp("PPid:", parola) == 0){
                        fscanf(gostatus,"%s", parola);
                        strcpy(pro->ppid, parola);
                        
                    }
                    if(strcmp("Name:", parola) == 0){
                        fscanf(gostatus,"%s", parola);
                        strcpy(pro->name, parola);
                        
                    }

                    //aggiungo le struct all'array
                    elenco[n] = malloc(sizeof(struct datiproc));
                    elenco[n] = pro;
                    n++;
                    elenco = realloc(elenco,(sizeof(struct datiproc*)*(n+1))); 
                }

                int j;
                for(j=0;j<nproc;j++){
                  //mvprintw(j+2,0,"%8s%8s%8s%20s\n", elenco[j]->pid, elenco[j]->ppid, elenco[j]->vmrss, elenco[j]->name);
                  printw("|%8s|%8s|%8s|%20s |\n", elenco[j]->pid, elenco[j]->ppid, elenco[j]->vmrss, elenco[j]->name);              
                }
                
                fclose(gostatus);

            }
        }
    }

    closedir(des);
}




void calcpu(char *percorso){


    FILE *gostat;
    char parola[1024];

    //variabile per calcolo CPU total
    double totalCPU;
    double totalCPUafter; 

    double temp = 0;

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
                        exit;
                    }

                    temp = atof(parola);
                    totalCPU += temp;
                    
                }
                printf("%f\n", totalCPU );
                
                fclose(gostat);

            }
        }
    }

    closedir(dir2);
}



int main()
{
    //printf("#######################################################\n");
    //printf("%8s%8s%8s%20s\n", "<PID>","<PPID>","<VmRSS>","<Name>");
    // IMPORTANTE!!! specificare percorso, ora metto questo per provare poi si dovrà mettere /proc/ 
    //printdir("/Users/Andrea/Desktop/");
    //printf("#######################################################\n");

    //carattere preso dalla getch
    char car;

    initscr();
    printw("#######################################################\n"); 
    printw(" %8s %8s %8s %20s  \n", "<PID>","<PPID>","<CPU>","<Name>");
    movedir("/Users/Andrea/Desktop/");

    refresh();  
    car = getch();  
    endwin();
    printf("%c\n",car); 
    return 0; 
}


