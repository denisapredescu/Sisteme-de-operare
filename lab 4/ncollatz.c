#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    printf("Starting parent %d\n", getpid());   //procesul parinte 

    int nr;
    pid_t pid;
    
    for(int i = 0; i < argc - 1; i++)  // ./ncollatz 9 16 25 36 => argc = 5. Noi vrem sa stim doar cate numere sunt, adica 4 = argc-1
    { 
        pid= fork();     
        if (pid<0)
            return errno;
        else
            if(pid==0)
            {
                printf("Done Parent %d Me %d\n", getppid(), getpid());

                nr = atoi(argv[i + 1]); //argv ul il iau incepand cu pozitia 1
                printf("%d: ", nr);
                while (nr != 1)   //fac functia collatz        
                {
                    printf("%d ",nr);
                    if(nr%2==0)
                        nr/=2;
                    else
                        nr=3*nr+1;
                } 
                printf("%d.\n",nr);
                break;       //resetez forkul   --fiecare fork va avea un singur proces copil
            }                //in total sunt doar (argc-1) copii
    }
    if(pid>0)
        {
            for(int i=0;i< argc - 1; i++)
                 wait(NULL);   

            printf("Done Parent %d Me %d\n", getppid(), getpid());
        }
    return 0;
}






