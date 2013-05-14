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
        //fprintf(stderr,"cannot open directory: %s\n", dir);
        perror(dir);
        return;
    }
    chdir(dir);

    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {

            /* Cerca una directory e ignora . e .. */
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0){
            	continue;
            }

            printf("%*s%s/\n",depth,"",entry->d_name);
            /* Ricorsione con tabulazione */
            printdir(entry->d_name,depth+4);
        }
        else printf("%*s%s\n",depth,"",entry->d_name);
    }
    chdir("..");
    closedir(dp);
}

int main()
{
    printf("Sto guardando nella directory: ./:\n");
    printdir("/Users/Andrea/Desktop",0);
    printf("FINE!\n");
    exit(0);
}