// Scrieti un program mycp care sa primeasca la intrare in primul argument 
// un fisier sursa pe care sa-l copieze intr-un fisier cu numele primit 
// in al doilei argument.
// Exemplu apel : ./mycp foo bar

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

int main(int arg, char* argv[])
{
    char* src_path = argv[1];
    char* dst_path = argv[2];

    struct stat stat_buf;
    if(stat(src_path, &stat_buf) == -1) //stat returneaza informatii
    {
        perror("stat failed at src");
        return errno;
    }

    int fd; 
    fd=open(src_path, O_RDONLY);    //deschide fisierul
    if(fd == -1)    
    {
        perror("open failed at src");
        return errno;
    }

    //vrem sa citim intr-un string textul din fisier

    char* buf = (char*) malloc (stat_buf.st_size * sizeof(char) +1);

    if(buf == NULL)
    {
        perror("malloc failed");
        return errno;
    }

    size_t bytes_read;
    for(int offset = 0; offset < stat_buf.st_size; offset += bytes_read)
    {
        bytes_read = read(fd, buf + offset, stat_buf.st_size - offset);
        if(bytes_read == -1)
        {
            perror("read failed");
            return errno;   
        }
        else if(bytes_read == 0)    //nu mai are ce citi, finalul file ului
                bytes_read = stat_buf.st_size;
    }
    buf[stat_buf.st_size] = '\0'; //tb sa pun null la final

    //O_TRUNC are grija ca, in cazul in care fisierul exista si a
    //fost deschis cu succes, sa truncheze lungimea datelor din el la 0 (sterg datele ce se aflau deja in fisier)
    
    int fd_write = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU); 
    if(fd_write == -1)                                               
    {
        perror("open failed at destination path");
        return errno;
    }
    
    size_t bytes_write;
    for(int offset = 0; offset < stat_buf.st_size; offset += bytes_write)
    {
        bytes_write = write(fd_write, buf + offset, stat_buf.st_size - offset);
       
        if(bytes_write == -1)
        {
            perror("write failed");
            return errno;
        }
        else if(bytes_write == 0)
            bytes_write = stat_buf.st_size;
    }

    free(buf);
    if (close(fd) == -1)
    {
        perror("close function failed");
        return errno;
    }
    if (close(fd_write) == -1)
    {
        perror("close function failed");
        return errno;
    }
    return 0;
}
