#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "makelog.h"

#define DIRECTORY 1
#define FILE 0
#define FIND 1
#define NOTFIND 0
#define NOT_EXIST -1

struct Object
{
  char nome[256];
  int tipo;
  int find;
};

struct Object** scrutadir(char *dir, int* dim);
int trova(struct Object** contenuto1, struct Object** contenuto2, int dim1, int dim2, char* a1, char* a2);
int isDir(char* dir);
int fileCompare(char* argv1, char* argv2);
int isStampable(const char c);
char argomento[20];


int main (int argc,char* argv[]){

	int dim1;
	int dim2;

  struct Object** contenuto1;
  struct Object** contenuto2;
  strcpy(argomento,argv[0]);
	if(argc < 3){
		printf("Mancano argomenti! Passare due file o due cartelle!\n");
    initlog(argv[0],NULL,NULL,NULL);
    writeERROR(argv[0],"Mancano argomenti! Passare due file o due cartelle!");
		exit(EXIT_FAILURE);
	}	
	else if(argc > 3){
		printf("Troppi argomenti! Passare due file o due cartelle!\n");
    initlog(argv[0],argv[1],argv[2],argv[3]);
    writeERROR(argv[0],"Troppi argomenti! Passare due file o due cartelle!");
		exit(EXIT_FAILURE);
  }
	else{
    initlog(argv[0],argv[1],argv[2],NULL);
    int firstISdir = isDir(argv[1]);
    int secondISdir = isDir(argv[2]);
    int allequal = 0;

     //caso: cartella----cartella
    if(firstISdir && secondISdir){  		
      //salvo directory corrente
  		char cwd[1024];
  		getcwd(cwd, sizeof(cwd));

  	  contenuto1 = scrutadir(argv[1], &dim1);
      chdir(cwd);//ritorno all'origine
  		contenuto2 = scrutadir(argv[2], &dim2);
      chdir(cwd);//ritorno all'origine
  		
      int i;
      int equal;
      equal = trova(contenuto1,contenuto2,dim1,dim2,argv[1],argv[2]);
      if(!equal){
        allequal = 1;
        //-----------------stampo risultato-------------------------
        //stampando i file che NON compaiono nell'altra directory
        int dim;
        if(dim1>dim2)
          dim = dim1;
        else
          dim = dim2;

        for(i=0; i<dim; i++){
          if(i<dim1){
            if(!contenuto1[i]->find)
              printf("Solo in %s:  %s\n", argv[1], contenuto1[i]->nome);
          }
          if(i<dim2){
            if(!contenuto2[i]->find)
              printf("Solo in %s:  %s\n", argv[2], contenuto2[i]->nome);
          }
        }
      }
      if(allequal == 0)
        writeOUTPUT(argomento,"File confrontati con successo. Sono uguali");
      else
        writeOUTPUT(argomento,"File confrontati con successo. Sono diversi");
      return allequal;
    }

    //caso: cartella----file
    if(firstISdir && !secondISdir){
      int openf = open(argv[2],O_RDONLY);
      if(openf==NOT_EXIST){
        perror(argv[2]); //ritorno errore argv[2] inesistente
        writeERROR(argv[0],"Secondo file inesistente");
        exit(EXIT_FAILURE);
      }
      else{
        printf("Il file %s è una directory, mentre il file %s è un file normale\n", argv[1], argv[2]);
        writeOUTPUT(argomento,"File confrontati con successo. Sono di tipo diverso");
        return 1;
      }
    }
    
    //caso: file----cartella
    if(!firstISdir && secondISdir){
      int openf = open(argv[1],O_RDONLY);
      if(openf==NOT_EXIST){
        perror(argv[1]); //ritorno errore argv[1] inesistente
        writeERROR(argv[0],"Primo file inesistente");
        exit(EXIT_FAILURE);
      }
      else{
        printf("Il file %s è un file normale, mentre il file %s è una directory\n", argv[1], argv[2]);
        writeOUTPUT(argomento,"File confrontati con successo. Sono di tipo diverso");
        return 1;
      }
    }

    //caso: file----file
    if(!firstISdir && !secondISdir){
      int equal = fileCompare(argv[1],argv[2]);
      if(equal != 1){
        if(equal==0)
          printf("I file %s e %s sono diversi\n", argv[1], argv[2]);
        allequal = 1;
      }
      if(allequal == 0)
        writeOUTPUT(argomento,"File confrontati con successo. Sono uguali");
      else
        writeOUTPUT(argomento,"File confrontati con successo. Sono diversi");
      return allequal;
    }
  }
  //----------------------------------------------------------
}

//scruta la directory settando il tipo. find viene settato a false
struct Object** scrutadir(char *dir, int* dim){
    int i=0;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        perror(dir);
        writeERROR(argomento,"Impossibile aprire la cartella");
        return;
    }
    chdir(dir);
    struct Object** contenuto = malloc(sizeof(struct Object*));
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        contenuto = realloc(contenuto,sizeof(char*)*(i+1));
        contenuto[i] = malloc(sizeof(struct Object));
        // controllo se ci sono elementi che iniziano con lettere o elementi .<nomefile> (nascosti) e li ignoro

        if(S_ISDIR(statbuf.st_mode)) {
            // Cerca una directory e ignora . e .. e se li trovo riparto con il ciclo
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0){
                continue;
            }

            // stampa le directory contenute nel path dato, aggiunge "/" cosi si identificano dai file
            //printf("%s/\n",entry->d_name);
            strcpy(contenuto[i]->nome,entry->d_name);
            contenuto[i]->tipo = DIRECTORY;
            contenuto[i]->find = NOTFIND;

            // Ricorsione con tabulazione
            //printdir(entry->d_name,depth+4);
        }
        // stampa i file contenuti nel path dato
        else{ //printf("%s\n",entry->d_name);
            strcpy(contenuto[i]->nome,entry->d_name);
            contenuto[i]->tipo = FILE;
            contenuto[i]->find = NOTFIND;
        }
        i++;
    }
    closedir(dp);
    (*dim) = i;
    return contenuto;
}

int trova(struct Object** contenuto1, struct Object** contenuto2, int dim1, int dim2, char* a1, char* a2){
    int i;
    int allequal = 1;
    int k=0;
    if(dim1>0 && dim2>0){
      for(i=0; i<dim1; i++){
        k=0;
        while(k<dim2){
          //confronto i file che hanno lo stesso nome nelle 2 directory
          //caso cartella----cartella----stesso nome
            if(strcmp(contenuto1[i]->nome,contenuto2[k]->nome)==0){
              if(contenuto1[i]->tipo == DIRECTORY && contenuto2[k]->tipo == DIRECTORY){
                contenuto1[i]->find = FIND;
                contenuto2[k]->find = FIND;
                k=dim2;
              }
            //caso cartella----file----stesso nome
            else if(contenuto1[i]->tipo == DIRECTORY && contenuto2[k]->tipo == FILE){
              contenuto1[i]->find = FIND;
              contenuto2[k]->find = FIND;
              printf("Il file %s in %s è una directory, mentre il file %s in %s è un file normale\n", contenuto1[i]->nome, a1, contenuto2[k]->nome, a2);
              k=dim2;
              allequal = 0;
            }
            //caso file----cartella----stesso nome
            else if(contenuto1[i]->tipo == FILE && contenuto2[k]->tipo == DIRECTORY){
              contenuto1[i]->find = FIND;
              contenuto2[k]->find = FIND;
              printf("Il file %s in %s è un file normale, mentre il file %s in %s è una directory\n", contenuto1[i]->nome, a1, contenuto2[k]->nome, a2);
              k=dim2;
              allequal = 0;
            }
            //caso file----file----stesso nome
            else if(contenuto1[i]->tipo == FILE && contenuto2[k]->tipo == FILE){
                contenuto1[i]->find = FIND;
                contenuto2[k]->find = FIND;
                char path1[1024];
                char path2[1024];
                strcpy(path1,a1);
                strcpy(path2,a2);
                if(path1[strlen(path1)-1] != '/')
                  strcat(path1,"/");
                if(path2[strlen(path2)-1] != '/')
                  strcat(path2,"/");
                strcat(path1,contenuto1[i]->nome);
                strcat(path2,contenuto2[k]->nome);
                int equal = fileCompare(path1, path2);
                if(equal != 1){
                  if(equal==0)
                    printf("I file %s e %s sono diversi\n", path1, path2);
                  printf("ssasdds    che cazzo succede?\n");
                  allequal=0;
                }
              }
            }
            else
              allequal = 0;
            k++;
        }
      }
    }
    else if(dim1 != dim2)
      allequal = 0;
    if(dim1==dim2 && dim1 == 0)
      allequal=1;
    return allequal;
}
//guarda se dir é una directory....in tal caso ritorna DIRECTORY, else FILE
int isDir(char* dir){
    int type = DIRECTORY;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL)
      type = FILE;
    else
      closedir(dp);
    return type;
}
//confronta due file, stampa le differtenze se sono stampabili
int fileCompare(char* argv1, char* argv2){
    int first = open(argv1,O_RDONLY);
    int second = open(argv2,O_RDONLY);

    int canread1, canread2; //servono per sapere quando devo smettere di leggere
    
    int equal = 1; //serve per vedere se  i 2 file sono uguali

    if(first==NOT_EXIST){
      perror(argv1); //ritorno errore argv1 inesistente
      writeERROR(argomento,"Primo parametro inesistente");
      if(second==NOT_EXIST){
        perror(argv2); //ritorno errore argv2 inesistente
        writeERROR(argomento,"Parametri inesistenti");
        exit(EXIT_FAILURE);
      }
    }
    if(second==NOT_EXIST){
      perror(argv2); //ritorno errore argv2 inesistente
      writeERROR(argomento,"Secondo parametro inesistente");
      exit(EXIT_FAILURE);
    }
    if(first!=NOT_EXIST && second!= NOT_EXIST){

      char ch1[1];
      char ch2[1];
    
      int i=0; //usata per ciclare

      char* diff1 = malloc(sizeof(char));
      char* diff2 = malloc(sizeof(char));

      int h =0;
      int dim1;
      int dim2;
      int canstamp=1;
      canread1 = read(first,ch1,1);
      canread2 = read(second,ch2,1);
      //controllo bit a bit
      while(canread1>0 && canread2>0 && !(equal == 0 && canstamp == 0)){
        if(ch1[0]!=ch2[0]){
          equal=0;
          diff1 = realloc(diff1,sizeof(char)*(h+1));
          diff2 = realloc(diff2,sizeof(char)*(h+1));
          diff1[h] = ch1[0];
          diff2[h] = ch2[0];
          h++;
          canstamp = isStampable(ch1[0])*isStampable(ch2[0]);
        }
        canread1 = read(first,ch1,1);
        canread2 = read(second,ch2,1);
      }
      dim1=h-1;
      dim2=h-1;
      //se file1 è in eccesso salvo altri caratteri
      if(canread1>0){
        equal=0;
        while(canread1>0 && canstamp != 0){
          diff1 = realloc(diff1,sizeof(char)*(h+1));
          diff1[h] = ch1[0];
          canstamp = isStampable(ch1[0]);
          h++;
          canread1 = read(first,ch1,1);
        }
        dim1 = h;
      }
      //se file2 è in eccesso salvo altri caratteri
      if(canread2>0){
        equal=0;
        while(canread2>0 && canstamp != 0){
          diff2 = realloc(diff2,sizeof(char)*(h+1));
          diff2[h] = ch2[0];
          canstamp = isStampable(ch2[0]);
          h++;
          canread2 = read(second,ch2,1);
        }
        dim2 = h;
      }     
      close(first);
      close(second);
      if(equal){ // se equal vale 1 sono uguali e ritorna 1
        return equal; 
      }
      else{ // se equal vale 0 sono diversi e ritorna 0 se non sono stampabili, 2 atrimenti
        if(canstamp){
          if(dim1>0){
            printf("\n------------------------------------------------%s---------------------------------------\n",argv1);
            for(i=0; i<dim1; i++)
              printf("%c", diff1[i]);
          }
          if(dim2>0){
            printf("\n------------------------------------------------%s---------------------------------------\n",argv2);
            for(i=0; i<dim2; i++)
              printf("%c", diff2[i]);
          }
          printf("\n----------------------------------------------------------------------------------------------\n");
          return 2;
        }
        else
          return equal;
      }
    }
}

//ritorna 1 se i caratteri sono stampabili, altrimenti 0
int isStampable(const char c){
  int stamp =1;
  if(c<32){
    if(c!=10)
      stamp = 0;
  }
  if(c>126 || c>126)
    stamp = 0;
  return stamp;
}