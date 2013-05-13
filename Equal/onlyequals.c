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

		int ci;
		if(first==NOT_EXIST){
			perror(argv[1]); //ritorno errore argv[1] inesistente
			//exit
		}
		if(second==NOT_EXIST){
			perror(argv[2]); //ritorno errore argv[1] inesistente
			//exit
		}
		if(first!=NOT_EXIST && second!= NOT_EXIST){

			char riga1[dim];
			char riga2[dim];
		
			int i=0; //usata per ciclare

			reset(riga1,riga2);
			while((canread1 = read(second,riga1,dim))>0 && (canread2 = read(first,riga2,dim))>0){
				int y;//usata per il ciclo
				for(y=0; y<dim; y++){
					//printf("f1: %c, f2: %c\n", riga1[y], riga2[y]);
					if(riga1[y] != riga2[y]){
						allequal = 0;
					}
				}
				i++;
				ci = i;
			}
			close(first);
			close(second);
			if(allequal){ // se allequal vale 1 sono tutti uguali e ritorna 0
				printf("I file sono uguali\n");
				return (0);	
			}else{ // se allequal vale 0 sono tutti uguali e ritorna 1
				printf("I file sono diversi\n");
				return (1);
			}
		}
	}
}

void reset(char* riga1, char*riga2){
	int i;
	for(i=0; i<dim; i++){
		riga1[i] =0;
		riga2[i] =0;
	}
}