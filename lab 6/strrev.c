#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


void * thread_routine(void* arg)
{
    char* sir_intors = malloc(sizeof(char));
    strcpy(sir_intors, (char*)arg);   ///copiez ce aveam in arg in sir_intors
                                      ///cu strcpy copiez tot stringul din (char*)arg in sir_intors. 
    int i = 0;
    int j = strlen(sir_intors)-1;
    char c;

    while (i < j)
    {
        c = sir_intors[i];
        sir_intors[i] = sir_intors[j];
        sir_intors[j] = c;
        i++;
        j--;
    }
    return sir_intors;
}

int main(int argc, char* argv[])
{
    pthread_t thread;
    //printf("Sirul %s\n", argv[1]);
    void* sir;
    if(pthread_create(&thread, NULL, thread_routine, argv[1]))   
    {
        perror(NULL);
        return errno;
    }

    if(pthread_join(thread, &sir))
    {
        perror(NULL);
        return errno;
    }

    printf("%s\n", (char*)sir);   //convertesc din void* la char*

    free(sir);  //in functie am folosit malloc, acum eliberez zona de memorie
    return 0;
}