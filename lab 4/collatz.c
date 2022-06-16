#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


int main(int arg, char *argv[])
{
    int nr = atoi(argv[1]);   //transform in int valoarea din argv[1]
    pid_t pid= fork();

    if (pid<0)
        return errno;
    else
        if(pid==0)
        {
            //fac functia collatz
            printf("%d: ", nr);
            while (nr != 1)
            {
                printf("%d ",nr);
                if(nr%2==0)
                    nr/=2;
                else
                    nr=3*nr+1;
            } 
            printf("%d \n",nr);
            
        }
        else
            if(pid>0)
            {
                wait(NULL);
                printf("Child %d finished\n", pid);
            }

    return 0;
}






