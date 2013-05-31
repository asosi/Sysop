#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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

			char* ch1 = malloc(sizeof(char));
			char* ch2 = malloc(sizeof(char));
		
			int i=0; //usata per ciclare

			char* diff1 = (char*)malloc(sizeof(char)*100);
			char* diff2 = (char*)malloc(sizeof(char)*100);

			int h =0;
			canread1 = read(first,ch1,1);
			canread2 = read(second,ch2,1);
			while(canread2>0){
				allequal=0;
				printf("%c     h vale %d\n", *ch2,h);
				//diff2 = realloc(diff2,sizeof(char)*(h+1));
				diff2[h] = *ch2;
				h++;
				canread2 = read(second,ch2,1);
			}
			close(first);
			close(second);
			if(allequal){ // se allequal vale 1 sono tutti uguali e ritorna 0
				printf("I file sono uguali\n");
				return (0);	
			}else{ // se allequal vale 0 sono tutti uguali e ritorna 1
				printf("I file sono diversi\n");
					printf("%d\n", strlen(diff2));
					printf("------------------------------------------------%s---------------------------------------\n%s",argv[1],diff1);
					printf("------------------------------------------------%s---------------------------------------\n%s",argv[2],diff2);
				return (1);
			}
		}
	}
}