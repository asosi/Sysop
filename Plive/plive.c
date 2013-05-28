#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

void proc(char *path);
void printdir(char *dir, int depth){

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        // fprintf(stderr,"cannot open directory: %s\n", dir);
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
            printf("%*s%s/\n",depth,"",entry->d_name);

            
            //concateno la stringa della dir con la cartella per creare il path di ogni cartella dei proc
            char new_path[256];
            new_path[0] = '\0';  // ensures the memory is an empty string
            strcat(new_path,dir);
            strcat(new_path,entry->d_name);
            strcat(new_path,"/");
            //printf("%s\n",new_path);

            //chiamo la funzione che entra nella sottodirectory del processo
            proc(new_path);

            // Ricorsione con tabulazione
            //printdir(entry->d_name,depth+4);
        }
        // stampa i file contenuti nel path dato
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);
}


void proc(char *path){

    DIR *des;
    struct dirent *entry;
    struct stat statbuf;
    if((des = opendir(path)) == NULL) {
        // fprintf(stderr,"cannot open directory: %s\n", path);
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
            printf("%s\n",entry->d_name);

            //controllo se nella directory in cui mi trovo esiste un file chiamato "status"
            if(strcmp("status",entry->d_name) == 0){
                printf("%s\n", "OK c'è il file status!");


                //int let = open(entry->d_name, O_RDONLY);
                //while(leggo = read(let,riga1,1))>0
            }
        }
    }

    chdir("..");
    closedir(des);
}

int main()
{
    // IMPORTANTE!!! specificare percorso, ora metto questo per provare poi si dovrà mettere /proc/ 
    printdir("/Users/Andrea/Desktop/",0);
    printf("FINE!\n");
    exit(0);
}