#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

int main()
{

    pid_t pid = fork();
    
    if(pid<0)
        return errno;
    else
        if(pid == 0)
    {
        const char * path = "/usr/bin/ls";
        char * argv[] = {"ls", NULL};

        execve (path, argv, NULL);  //execve face ca procesul copil sa fie suprascris de programul apelat
    }
        else 
            if (pid>0)
            {
                printf("My PID = %d, Child PID = %d\n",getpid(),pid);
                wait(NULL);
                printf("Child %d finished \n", pid);
            }
    

    return 0;

}