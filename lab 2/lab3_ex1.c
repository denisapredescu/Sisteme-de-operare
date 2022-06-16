#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

int main(int arg, char* argv[])
{
    char* src_path = argv[1];
    
    struct stat _buf;   //vreau sa vad cate caractere sunt in helloworld.c
    if (stat(src_path, &_buf) == -1)     //tratez eroarea stat
    {
        perror("stat failed at src");   //daca cumva nu se poate lua fisierul din care vreau sa citesc
        return errno;
    }

    int fd;
    fd = open(src_path, O_RDONLY);  //deschid fisierul helloworld
    if(fd == -1 )   //tratez eroarea ce poate aparea la deschiderea fisierului
    {
        perror("openning file error");
        return errno;
    }


    char* b = (char*)malloc(_buf.st_size * sizeof(char*)+1);

    if(b == NULL)       //tratez eroarea de la malloc
    {
        perror("malloc error");
        return errno;
    }

    size_t bytes_read;
    for(int offset = 0; offset < _buf.st_size; offset += bytes_read)
    {
        bytes_read = read(fd, b + offset, _buf.st_size);
        if(bytes_read == -1)     
        {
            perror("read failed");   //daca cumva nu se poate lua fisierul din care vreau sa citesc
            return errno;
        }
        else if(bytes_read == 0)    //nu mai are ce citi, finalul file ului
            bytes_read = _buf.st_size; 

    }
    
    int bytes_write;
    for(int offset = 0; offset < _buf.st_size; offset += bytes_write)
    {
        bytes_write = write(STDOUT_FILENO, b + offset, _buf.st_size);
        if(bytes_write == -1)     
        {
            perror("write failed");   //tratam write error
            return errno;
        }
        else if(bytes_write == 0)    //nu mai are ce scrie, finalul file ului
            bytes_write = _buf.st_size;

    }

    free(b);
    if (close(fd) == -1)
    {
        perror("close function failed");
        return errno;
    }
    return 0;
}