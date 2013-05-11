#include <stdio.h>
#include <fcntl.h>
//****************************IMPORTANTE*******************************************
int dim = 512;//cosa mettiamo? ho scelto 512, accetto consigli

int main (int argc,char* argv[]){

	if(argc < 3)
		printf("Mancano argomenti! Passare due file o due cartelle!\n");
	else if(argc > 3)
		printf("Troppi argomenti! Passare due file o due cartelle!\n");
	else{
		int first = open(argv[1],O_RDONLY);
		int second = open(argv[2],O_RDONLY);

		int canread1, canread2; //servono per sapere quando devo smettere di leggere
		
		int allequal = 1;
		char riga1[dim];
		char riga2[dim];
		int ci;
		int i=0; //usata per ciclare
		while((canread1 = read(second,riga1,dim))>0 && (canread2 = read(first,riga2,dim))>0){
			int y;//usata per il ciclo
			for(y=0; y<dim; y++){
				if(riga1[y] != riga2[y])
					allequal = 0;
			}
			i++;
			ci = i;
		}

		close(first);
		close(second);
		
		if(allequal){ // se allequal vale 1 sono tutti uguali e ritorna 0
			printf("I file sono uguali\n");
			return 0;		
		}else{ // se allequal vale 0 sono tutti uguali e ritorna 1
			printf("I file sono diversi, ho ciclato %i volte\n", ci);
			return 1;
		}
	}
}