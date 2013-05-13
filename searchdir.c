    #include <stdio.h>
    #include <string.h>
    #include <dirent.h>
    #include <sys/types.h>
    #include <stdlib.h>

    #define TRUE   1
    #define FALSE   !TRUE


    /*
      QUESTA E' QUELLA CHE AVEVO TROVATO E CHE VI DICEVO, cerca nella cartella che passi i file e gli elenca, 'è la possibilità'
      di scegliere se cercare nella cartella al primo livello o di andare anche in sottocartelle (se è come la diff terrei primo livello).
      Ora che si elencano i file invece che stamparli sarà da fare un controllo usando la onlyequals.c
    */

    typedef unsigned short int boolean;

    char **find_directory(char *, unsigned long int *, boolean); //restituisce tutti i file della directory indicata

    int main(int argc, char **argv){

       char **files;
       unsigned long int num, i;
       
       files = find_directory(argv[1], &num, TRUE); //imposta TRUE per cercare anche nelle sottodirectory
                                                    //FALSE per cercare solo nella cartella indicata
       
       for(i = 0; i < num; i++)printf("File: %s\n", files[i]);
       for(i = num - 1; i > -1; i--)free(files[i]); //dealloco la memoria
       
       return 0;
    }

    char **find_directory(char *name, unsigned long int *size, boolean subfolder){

       struct dirent **dirent;
       DIR *dir;
       
       char **files, **directory;
       char buffer[260];
       
       unsigned long int i, n, sub = 0;
       unsigned long int num = scandir(name, &dirent, 0, alphasort); //trova tutti i files e cartelle
       
       *size = 0;
       
       for(i = 0; i < num; i++){
       
          if(strcmp(dirent[i]->d_name, ".") != 0 && strcmp(dirent[i]->d_name, "..") != 0){
          
             snprintf(buffer, 260, "%s/%s", name, dirent[i]->d_name);
             if((dir = opendir(buffer)) != NULL){

                //trovata una sottocartella
                closedir(dir);
                
                if(!subfolder)continue;
                
                if(!sub){
                   
                   directory = (char **)malloc(sizeof(char **));
                   directory[sub] = (char *)malloc(sizeof(char *) * 260);
                   
                }else{
                
                   directory = (char **)realloc(directory, sizeof(char **) * (sub + 1));
                   directory[sub] = (char *)malloc(sizeof(char *) * 260);
                }
                
                snprintf(directory[sub], 260, "%s", buffer);
                ++sub;
                
             }else{
             
                //trovato file
                if(!(*size)){
                
                   files = (char **)malloc(sizeof(char **));
                   files[*size] = (char *)malloc(sizeof(char *) * 260);
                   
                }else{
                
                   files = (char **)realloc(files, sizeof(char **) * (*size + 1));
                   files[*size] = (char *)malloc(sizeof(char *) * 260);
                }
                
                snprintf(files[*size], 260, "%s", dirent[i]->d_name);
                ++(*size);
             }
          }
       }
       
       //cerca nelle eventuali sottodirectory
       if(sub && subfolder){
       
          for(i = 0; i < sub; i++){
          
             num = scandir(directory[i], &dirent, 0, alphasort);
             
             for(n = 0; n < num; n++){
             
                if(strcmp(dirent[n]->d_name, ".") != 0 && strcmp(dirent[n]->d_name, "..") != 0){
                
                   snprintf(buffer, 260, "%s/%s", directory[i], dirent[n]->d_name);
                   if((dir = opendir(buffer)) != NULL){
                   
                      //trovata ulteriore sottodirectory
                      closedir(dir);
                      
                      directory = (char **)realloc(directory, sizeof(char **) * (sub + 1));
                      directory[sub] = (char *)malloc(sizeof(char *) * 260);
                
                      snprintf(directory[sub], 260, "%s", buffer);
                      ++sub;
                   }else{
                   
                      //trovato file
                      if(!(*size)){
                
                         files = (char **)malloc(sizeof(char **));
                         files[*size] = (char *)malloc(sizeof(char *) * 260);
                   
                      }else{
                
                         files = (char **)realloc(files, sizeof(char **) * (*size + 1));
                         files[*size] = (char *)malloc(sizeof(char *) * 260);
                      }
                
                      snprintf(files[*size], 260, "%s", dirent[n]->d_name);
                      ++(*size);
                   }
                }
             }
          }
       }
       
       for(i = sub - 1; i > -1; i--)free(directory[i]); //dealloca la memoria
       
       return files;
    }

