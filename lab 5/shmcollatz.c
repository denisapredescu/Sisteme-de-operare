#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

//compilez cu gcc shmcollatz.c -lrt -o shmcollatz 

#define SHM_SIZE 1024

int lung (int n)   //cat spatiu ocupa numarul
{
    int z = 0;
    while (n != 0)
    {
        n = n / 10;
        z = z + 1;
    }
    return z;
}

int main(int arg, char* argv[])
{
    printf("Starting parent %d\n", getpid());  //procesul initial este parinte

    const char * shm_name = "partajare";   //numele obiectului
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);  //crearea obiectului de memorie partajata

    if(shm_fd < 0){
        perror(NULL);
        return errno;
    }

    if(ftruncate(shm_fd, SHM_SIZE) == -1)  //dau dimensiune obiectului
    {
        perror(NULL);
        shm_unlink(shm_name);   //daca se intampla sa nu pot aloca dimensiunea dorita, trebuie sa sterg obiectul shm_name
        return errno;
    }

    char * shm_ptr_parent = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);   //pointerul va indica catre zona de memorie
                            //parintele doar citeste din zona

    if(shm_ptr_parent == MAP_FAILED)
    {
        perror(NULL);
        shm_unlink(shm_name);  //sterg obiectul
        return errno;
    }

    int offset = 100;           
    pid_t pid;

    for(int i = 0; i < arg - 1; i++)
    {
        pid = fork();
        if(pid < 0)
        {
            perror(NULL);
            munmap(shm_ptr_parent, SHM_SIZE);     //nu mai este nevoie de zona de memorie
            shm_unlink(shm_name);     //sterg obiectul
            return errno;
        }
        else
            if(pid == 0)
            {
                printf("Done Parent %d Me %d\n", getppid(), getpid());

                char * shm_ptr_child = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);   //copilul poate sa scrie - citeasca 

                    if(shm_ptr_child == MAP_FAILED)
                {
                    perror(NULL);
                    shm_unlink(shm_name);  
                    return errno;
                }

                int nr = atoi(argv[i + 1]);
                sprintf(shm_ptr_child + i * offset, "%d: ", nr);
                int lungime = lung(nr) + 2;      

                while(nr != 1)
                {
                    sprintf(shm_ptr_child + i * offset + lungime, "%d ", nr);
                    lungime = lungime + lung(nr) + 1;

                    if(nr%2==0)
                        nr = nr/2;
                    else
                        nr = 3*nr+1;
                }

                sprintf(shm_ptr_child + i * offset + lungime, "%d ", nr);  //il pun si pe 1
                munmap(shm_ptr_child, SHM_SIZE);     
                break;
            }
    }
    if(pid > 0)
    {
        for(int i = 0; i < arg - 1; i++)
            wait(NULL);      //astept ca toti copiii sa se termine

        for(int i = 0; i < arg - 1; i++)
        {
            printf("%s\n", shm_ptr_parent + i * offset);
        }

        printf("Done Parent %d Me %d\n", getppid(), getpid());
    }

    munmap(shm_ptr_parent, SHM_SIZE);  //unmap the file
    shm_unlink(shm_name);     //sterg obiectul creat

    return 0;
}