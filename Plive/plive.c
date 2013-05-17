#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

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

        // controllo se ci sono file .<nomefile> (nascosti) e li ignoro
        if(entry->d_name[0] == '.'){
            continue;
        }

        if(S_ISDIR(statbuf.st_mode)) {
            // Cerca una directory e ignora . e .. e se li trovo riparto con il ciclo
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || entry->d_name[0] == '.'){
            	continue;
            }

            // stampa le directory contenute nel path dato
            printf("%*s%s/\n",depth,"",entry->d_name);

            // Ricorsione con tabulazione
            printdir(entry->d_name,depth+4);
        }
        // stampa i file contenuti nel path dato e nelle sottodirectory
        else printf("%*s%s\n",depth,"",entry->d_name,"bo");
    }
    chdir("..");
    closedir(dp);
}

int main()
{
    printf("Sto guardando nella directory:\n");
    // specificare percorso, ora metto questo per provare poi si dovrà mettere /proc 
    printdir("/proc/",0);
    printf("FINE!\n");
    exit(0);
}