#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>  //pentru malloc/free
#include <semaphore.h>

#define NTHRS 5
pthread_mutex_t mutex;
sem_t sem;
int bariera;

void init(int N)
{
    bariera = N;
}

int barrier_point()
{ 
    pthread_mutex_lock(&mutex);   //variabila bariera poate fi modificata de un singur thread deodata
    bariera -= 1;  
    pthread_mutex_unlock(&mutex);

    if (bariera == 0) 
        if(sem_post(&sem)){  //deblocheaza semaforul, >0
            perror("sem_post failed"); 
            return errno; 
        }
    
    if(sem_wait(&sem)){             //valoarea initial din sem_val este 0; se blocheaza pana cand valoarea
        perror("sem_wait failed");  //va deveni pozitiva(>0) => atunci automat se va debloca si va trece
        return errno;               //la randul urmator, blocand insa semaforul pentru celelalte threaduri
    }
    if(sem_post(&sem)){  // deblocheaza semaforul pentru urmatorul thread    
        perror("sem_post failed"); 
        return errno; 
    }
    return 0;
}

void* tfun(void *v)
{
    int* tid = malloc(sizeof(int));
    *tid = *(int *)v;

    printf ("%d reached the barrier\n", *tid);
    barrier_point ();
    printf ("%d passed the barrier\n", *tid);
    
    free(tid);
    return NULL;
}

int main()
{
    init(NTHRS);   //initializez bariera cu numarul de threaduri
    pthread_t thread[NTHRS];
    int arg[NTHRS];   

    if (pthread_mutex_init(&mutex, NULL)) {
        perror ("Mutex_ init failed");
        return errno ;
    }

    int sem_val = 0;      //fiind 0, threadurile care ajung la sem_wait sunt blocate pana in momentul in care dau peste un sem_post care sa creasca valoarea
    if(sem_init(&sem, 0, sem_val)){
        perror ("sem_ init failed");
        return errno ;
    }

    for(int i = 0; i< NTHRS; i++)
    {
        arg[i] = i;
        pthread_create(&thread[i], NULL, tfun, &arg[i]);
    } 
    for(int i = 0; i< NTHRS; i++)   //astept cele NTHRS threaduri
    {
        pthread_join(thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex);   //eliberez mutexul si semaforul
    sem_destroy(&sem);

    return 0;
}