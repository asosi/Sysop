#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#define NOT_EXIST -1
//****************************IMPORTANTE*******************************************
int dim = 512;//cosa mettiamo? ho scelto 512, accetto consigli

void reset(char* riga1, char*riga2);

int main (int argc,char* argv[]){

	if(argc < 3){
		printf("Mancano argomenti! Passare due file o due cartelle!\n");
		//exit
	}	
	else if(argc > 3)
		printf("Troppi argomenti! Passare due file o due cartelle!\n");
		//exit
	else{
		int first = open(argv[1],O_RDONLY);
		int second = open(argv[2],O_RDONLY);

		int canread1, canread2; //servono per sapere quando devo smettere di leggere
		
		int allequal = 1; //serve per vedere se  i 2 file sono uguali

		if(first==NOT_EXIST){
			perror(argv[1]); //ritorno errore argv[1] inesistente
			//exit
		}
		if(second==NOT_EXIST){
			perror(argv[2]); //ritorno errore argv[1] inesistente
			//exit
		}
		if(first!=NOT_EXIST && second!= NOT_EXIST){

			char riga1[1];
			char riga2[1];
		
			int i=0; //usata per ciclare

<<<<<<< HEAD
			char* diff1 = (char*)malloc(sizeof(char));
			char* diff2 = (char*)malloc(sizeof(char));

			// :)

			//  X  X XXXX X    X    XXXX XXXX
			//  X  X X    X    X    X  X X
			//  XXXX XXX  X    X    XXXX XXXX
			//  X  X X    X    X    X  X    X
			//  X  X XXXX XXXX XXXX X  X XXXX
			int h =0;
			while((canread1 = read(first,riga1,1))>0 && (canread2 = read(second,riga2,1))>0){
				//printf("f1: %c, f2: %c\n", riga1[y], riga2[y]);
				if(riga1[0] != riga2[0]){
					allequal = 0; //setto non diversi
					diff1 = (char*)realloc(diff1,sizeof(char)*(h+1));
					diff2 = (char*)realloc(diff2,sizeof(char)*(h+1));
					diff1[h] = riga1[0];
					diff2[h] = riga2[0];
					h++;
				}
			}
			if(canread1>0){
					diff1 = (char*)realloc(diff1,sizeof(char)*(h+1));
					diff1[h] = riga1[0];
					h++;
				while((canread1 = read(first,riga1,1))>0){
					diff1 = (char*)realloc(diff1,sizeof(char)*(h+1));
					diff1[h] = riga1[0];
					h++;
				}
			}
			else if(canread2>0){
					diff2 = (char*)realloc(diff2,sizeof(char)*(h+1));
					diff2[h] = riga2[0];
					h++;
				while((canread2 = read(second,riga2,1))>0){
					diff2 = (char*)realloc(diff2,sizeof(char)*(h+1));
					diff2[h] = riga2[0];
					h++;
=======
			reset(riga1,riga2);
			while((canread1 = read(second,riga1,dim))>0 && (canread2 = read(first,riga2,dim))>0){
				int y;//usata per il ciclo
				for(y=0; y<dim; y++){
					//printf("f1: %c, f2: %c\n", riga1[y], riga2[y]);
					if(riga1[y] != riga2[y]){
						allequal = 0;
					}
>>>>>>> b27e6583f508250fb8b5dc8878131ebe043226a1
				}
			}
			close(first);
			close(second);
			if(allequal){ // se allequal vale 1 sono tutti uguali e ritorna 0
				printf("I file sono uguali\n");
				return (0);	
			}else{ // se allequal vale 0 sono tutti uguali e ritorna 1
				printf("I file sono diversi\n");
					printf("------------------------------------------------%s------------------------------------------------\n%s\n",argv[1],diff1);
					printf("------------------------------------------------%s------------------------------------------------\n%s\n",argv[2],diff2);
					free(diff1);
					free(diff2);
				return (1);
			}
		}
	}
}