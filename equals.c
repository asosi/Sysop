/*
	PROVA PROVA
*/ 
/*
ho provato ad aggiungere un commento
		BY EMPY92
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int dim = 256;
struct line{
	char* row;
	int trovato;
};

struct line** file1;
struct line** file2;

int main (int argc,char* argv[]){

	if(argc < 3 || argc > 3){

		printf("Errore, passare due file o due cartelle!\n");
		}else{
		FILE* first = fopen(argv[1],"r");
		FILE* second = fopen(argv[2],"r");

		int sizef1 =0;
		int sizef2 =0;

		int fine;
		char* rigatestuale = (char*)malloc(sizeof(char)*dim); 

		int i=0; //usata per ciclare
		while((fine = getline(&rigatestuale,&dim,second))!=EOF){
			file2 = (struct line**)realloc(file2, (i+1)*sizeof(struct line*)); //allocco l'array di puntatori a struct
			file2[i] = (struct line*)malloc(sizeof(struct line));
			file2[i]->row = (char*)malloc(sizeof(char)*dim);
			strcpy(file2[i]->row,rigatestuale);
			file2[i] -> trovato = 0;
			i++;

		}
		sizef2 = i; // setto size del file2

		i=0;
		while((fine = getline(&rigatestuale,&dim,first))!=EOF){
			file1 = (struct line**)realloc(file1, (i+1)*sizeof(struct line*)); //allocco l'array di puntatori a struct
			file1[i] = (struct line*)malloc(sizeof(struct line));
			file1[i]->row = (char*)malloc(sizeof(char)*dim);
			strcpy(file1[i]->row,rigatestuale);
			file1[i] -> trovato = 0;
			i++;
		}
		sizef1 = i; // setto size del file1

		fclose(first);
		fclose(second);
		
		//SIGN: mi ricorda dove sono arrivato nel primo file per controllare le righe sotto
		int sign=sizef1-1;
		int find = 1;
		int j; //usata per il ciclo
		for(i=sizef2-1;i >= 0;i--){
			find = 1;
			for(j=sign;j >= 0;j--){
				if(strcmp(file2[i]->row, file1[j]->row) == 0 && find){
					file1[j]->trovato = 1;
					file2[i]->trovato = 1;
					find = 0;
					sign = j-1;
				}
			}
		}
		int allequal = 1; //serve a verificare se tutte le righe sono uguali
		
		printf("-------------------------- %s -------------------------------\n", argv[1]);
		for(i=0;i<sizef1;i++){
			if(file1[i]->trovato == 0){
				printf("%s",file1[i]->row);
				allequal = 0;
			}
		}
		
		printf("-------------------------- %s -------------------------------\n", argv[2]);

		for(i=0;i<sizef2;i++){
			if(file2[i]->trovato == 0){
				printf("%s",file2[i]->row);
				allequal = 0;
			}
		}

		//deallocca
		free(rigatestuale);

		for(i=0; i<sizef2; i++){
			free(file2[i]->row);
			free(file2[i]);
		}
		free(file2);
		for(i=0; i<sizef1; i++){
			free(file1[i]->row);
			free(file1[i]);
		}
		free(file1);
		//fine deallocca

		if(allequal){ // se allequal vale 1 sono tutti uguali e ritorna 0
			printf("Valore ret: %d\n",0);
			return 0;		
		}else{ // se allequal vale 0 sono tutti uguali e ritorna 1
			printf("valore ret: %d\n",1);
			return 1;
		}
	}
}