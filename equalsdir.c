#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

char** scrutadir(char *dir, int* dim);
void mergesort(char** f, int start, int end);
void merge(char** f, int start, int center, int end);

//funzione che stampa il contenuto di una cartella
int main (int argc,char* argv[]){

	int dim1;
	int dim2;

    char** contenuto1;
    char** contenuto2;

	if(argc < 3){
		printf("Mancano argomenti! Passare due file o due cartelle!\n");
		//exit
	}	
	else if(argc > 3)
		printf("Troppi argomenti! Passare due file o due cartelle!\n");
		//exit
	else{
		//salvo directory corrente
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));

	    contenuto1 = scrutadir(argv[1], &dim1);
        chdir(cwd);//ritorno all'origine
		contenuto2 = scrutadir(argv[2], &dim2);

        mergesort(contenuto1,0,dim1-1);
        mergesort(contenuto2,0,dim2-1);
        
		int i;
		for(i=0; i<dim2; i++)
			printf("%s\n", contenuto2[i]);
		printf("\n");
		for(i=0; i<dim1; i++)
			printf("%s\n", contenuto1[i]);
	}
}

//scruta la directory aggiungendo "/" al nome di una cartella, "." al nome di un file
char** scrutadir(char *dir, int* dim){
    int i=0;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        perror(dir);
        return;
    }
    chdir(dir);
    char** contenuto = malloc(sizeof(char*));
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        contenuto = realloc(contenuto,sizeof(char*)*(i+1));
        contenuto[i] = malloc(sizeof(char)*(strlen(entry->d_name)));
        // controllo se ci sono elementi che iniziano con lettere o elementi .<nomefile> (nascosti) e li ignoro

        if(S_ISDIR(statbuf.st_mode)) {
            // Cerca una directory e ignora . e .. e se li trovo riparto con il ciclo
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0){
                continue;
            }

            // stampa le directory contenute nel path dato, aggiunge "/" cosi si identificano dai file
            //printf("%s/\n",entry->d_name);
            contenuto[i] = strcat(entry->d_name,"/");

            // Ricorsione con tabulazione
            //printdir(entry->d_name,depth+4);
        }
        // stampa i file contenuti nel path dato
        else //printf("%s\n",entry->d_name);
            contenuto[i] = strcat(entry->d_name,".");
        i++;
    }
    closedir(dp);
    (*dim) = i;
    return contenuto;
}

void merge(char** f, int start, int center, int end){
  int s = start;
  int c = center+1;
  char* supp[end-start];
  int i = 0;
  while(s<=center && c<=end){
    if(strcmp(f[s],f[c])<0){
      supp[i] = f[s];
      s++;
    }
    else{
      supp[i] = f[c];
      c++;
    }
    i++;
  }
  while(s<=center){
    supp[i] = f[s];
    s++;
    i++;
  }
  while(c<=end){
    supp[i] = f[c];
    c++;
    i++;
  }
  for(i=start; i<=end; i++)
    f[i] = supp[i-start];
}

void mergesort(char** f, int start, int end){
  if(start<end){
    int center = (start+end)/2;
    mergesort(f,start,center);
    mergesort(f,center+1,end);
    merge(f,start,center,end);
  }
}

int trova(char** contenuto1, char** contenuto2, int dim1, int dim2){
    int i=0;
    int k=0;
    while(k<dim2){
        if(strcmp(contenuto1[0],contenuto2[k])==0){
            k=dim2;
            printf("Trovato\n");
        }
        else
            k++;
    }
}