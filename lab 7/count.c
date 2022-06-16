#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;

#define NTHREAD 5
int resurse_cerute[NTHREAD] = {2,2,1,2,3};   //resursele dorite de threaduri


pthread_mutex_t mutex;

//folosesc un singur mutex. Daca ar fi fost 2 (unul pentru increase si unul pentru decrease) ar si insemnat 
//ca se permite modificarea variabilei in acelasi timp, pe doua threaduri diferite. Avand unul singur, 
//cand un thread va fi intr-una dintre functii, niciun alt thread nu va putea schimba valoarea variabilea.
   
int decrease_count ( int count )
{  
    pthread_mutex_lock(&mutex);  //blochez accesul threadurilor de aici pentru ca  variabilei available_resources din 
                                 //comparatia if(available_resources < count) sa nu i se poate schimba valoarea dupa 
                                 //intrarea in if de catre alt thread care ruleaza in paralel
   
    if ( available_resources < count) //daca nu ii pot da threadului numarul de resurse dorite, intorc -1
        { 
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    else
    {
        available_resources -= count ; 
        printf("Got %d resources %d remaining\n" , count, available_resources);  
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int increase_count ( int count )
{   
    pthread_mutex_lock(&mutex);

    available_resources += count ;
    printf("Released %d resources %d remaining\n" , count, available_resources);
    
    pthread_mutex_unlock(&mutex);

    return 0;
}

void* thread_routine (void*arg)
{
    int count =  *(int *)arg;  

    while(decrease_count(count)); //cat timp nu ii pot da unui thread numarul de resurse dorite, functia decrease_count va intoarce -1 
                                  //si threadul respectiv va astepta pana cand i se va putea da numarul de resurse dorite
                                  //duce ce i se dau, acesta poate sa le dea inapoi
    increase_count(count);
    
    return NULL;
}
int main()
{

    if(pthread_mutex_init(&mutex, NULL))
    {
        perror("Mutex init failed");
        return errno;
    }

    pthread_t thread[NTHREAD];     

    for(int i=0; i<NTHREAD; i++)
    {
        if(pthread_create(&thread[i], NULL, thread_routine, &resurse_cerute[i]))
        {
            perror("Pthread_create failed");
            return errno;
        }

    }
    for(int i=0;i<NTHREAD;i++)
    {
        if(pthread_join(thread[i],NULL))
        {
            perror("Pthread_join failed");
            return errno;
        }
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}