#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int count_occurrences(const char* text,const char* word)
{
	int c=0;
	while(text!=NULL)
	{
		text=strstr(text, word);
		if(text!=NULL){
	            c=c+1;
		        text = text+strlen(word);
        }
	}

	return c;
}
void replace(const char*inlocuit, const char*inlocuieste)
{
    char* text=(char*)malloc(256*sizeof(char));
    
    scanf("%s",text);
    
    int size=(int)strlen(text)+ (count_occurrences(text, inlocuit)*(int)strlen(inlocuieste));
    char *replaced=(char*)malloc(size*sizeof(char));    
   
    char * aux ;
    while(text!=NULL)
    {   
        aux=strstr(text, inlocuit);

        if(aux!= NULL)
        {
            char * primele= (char*)malloc((strlen(text)-strlen(aux))*sizeof(char)+1);
            strncpy(primele, text, strlen(text)-strlen(aux));
            
            primele[strlen(text)-strlen(aux)]='\0';
            //printf("Prima parte %s \n", primele);

            strcat(replaced, primele);
            //printf("prima lipitura %s \n", replaced);
            
            strcat(replaced,inlocuieste);
            //printf("A doua lipitura %s \n", replaced);
            
            strcpy(text, aux+ strlen(inlocuit));
            //printf("textul ramas %s \n", text);
            
            free(primele);
        }
        else
        {
            strcat(replaced, text);
            text = aux;
        }           
    }  
    free(text);
    
    printf("Textul dat de la tastatura devine %s \n", replaced);
    free(replaced);
}

int main()
{
    replace("a","ca");
    return 0;
}