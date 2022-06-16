#include <pthread.h>
#include <stdio.h>
#include <errno.h>


//teste
//gcc produs_matrice.c -pthread -w -o produs_matrice
//./produs_matrice

// #define m 2
// #define p 2
// #define n 2
// int a[m][p] = {
//             {1,2},
//             {3,4}
//             };
// int b[p][n] = {
//             {1,2},
//             {3,4}
//             };

// //matricea rezultata:
// //7 10 
// //15 22


// #define m 1
// #define p 2
// #define n 2
// int a[m][p] = {
//             {1,2}
//             };
// int b[p][n] = {
//             {1,5},
//             {5,6}
//             };

// // 11 17


// #define m 3
// #define p 3
// #define n 2
// int a[m][p] = {
//             {1,2,3},
//             {4,5,6},
//             {7,8,9}
//             };
// int b[p][n] = {
//             {1,2},
//             {1,3},
//             {1,4}
//             };

// // 6 20 
// // 15 47 
// // 24 74 


#define m 3
#define p 2
#define n 4
int a[m][p] = {
            {1,2},
            {2,2},
            {3,2}
            };
int b[p][n] = {
            {1,2,3,4},
            {5,6,7,8}
            };

// // 11 14 17 20 
// // 12 16 20 24 
// // 13 18 23 28 


int c[m][n];

struct thread_params {
    int ai,bj;
};

typedef struct thread_params thread_params;   // de acum structura va putea fi apelata doar cu thread_params

void * thread_routine(void* arg)
{
    thread_params* param = (thread_params*)arg;

    for(int i = 0; i< p;i++)
        c[param->ai][param->bj] = c[param->ai][param->bj] + a[param->ai][i] * b[i][param->bj];

    return NULL;
}

int main()
{

    pthread_t thread[m*n];          //creez un thread pt fiecare element din matricea finala
    thread_params rowcolumn[m*n];   //si o structura pentru fiecare apelare a functiei
    int nr_thread = 0;              //cu aceasta variabila voi parcurge threadurile si structurile

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){

            rowcolumn[nr_thread].ai = i;
            rowcolumn[nr_thread].bj = j;
    
            if(pthread_create(&thread[nr_thread],NULL, thread_routine, &rowcolumn[nr_thread]))    
            {
                perror("Pthread_create failed");
                return errno;
            }
            nr_thread++;
        }
    }

    for(int i = 0; i < m * n; i++){      
        if(pthread_join(thread[i], NULL))     //threadul initial asteapta sa se termine fiecare thread
        {
            perror("Pthread_join failed");
            return errno;
        }
    }

    for(int i = 0; i < m; i++){          //afisarea matricei finale
        for(int j = 0; j < n; j++){
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }
    return 0;
}