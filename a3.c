#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define FIFO_WRITE "RESP_PIPE_97699"
#define FIFO_READ "REQ_PIPE_97699"
#define CONNECT "CONNECT"
#define PING "PING"
#define PONG "PONG"
#define EXIT "EXIT"
#define SUCCESS "SUCCESS"
#define ERROR "ERROR"
#define CRT "CREATE_SHM"

unsigned int connect_nr = strlen(CONNECT);
unsigned int crt_nr = strlen(CRT);
unsigned int error_nr = strlen(ERROR);
unsigned int success_nr =strlen(SUCCESS);
unsigned int nr = 97699;

int main() {
    int fd_write = -1, fd_read = -1;
    
    unsigned int n, nr_sh;

    if(access(FIFO_WRITE, 0) == 0) {
        unlink(FIFO_WRITE);
    }

    //create fifo
    if(mkfifo(FIFO_WRITE, 0600) != 0) {
        printf("ERROR\ncannot create the response pipe\n");
    }


    fd_read = open(FIFO_READ, O_RDONLY);
    if(fd_read == -1) {
        printf("ERROR\ncannot open the request pipe\n");
    }

    //open, write and close fifo
    fd_write = open(FIFO_WRITE, O_WRONLY);

    if((write(fd_write, &connect_nr, 1) != -1) && (write(fd_write, &CONNECT, connect_nr) != -1)) {
        printf("SUCCESS\n");
    }
    
    
    while(1) {
        read(fd_read, &n, 1);
        char* p = (char*)malloc(n * sizeof(char));
        read(fd_read, p, n);
        //p[4] = '\0';
        //for(int i = 0; i < strlen(p); i++)
        //    printf("%c\n", p[i]);
        if(strcmp(p, "PING") == 0) {
            write(fd_write, &n, 1);
            write(fd_write, &PING, 4);
            write(fd_write, &n, 1);
            write(fd_write, &PONG, 4);
            write(fd_write, &nr, sizeof(nr));
        }
        if(strcmp(p, "EXIT") == 0) {
            close(fd_write);
            close(fd_read);
            free(p);
            p = NULL;

            //delete fifo
            unlink(FIFO_WRITE);
            return 0;
        }
        if(strcmp(p, "CREATE_SHM") == 0) {
            read(fd_read, &nr_sh, sizeof(nr_sh));

            int shmFd = shm_open("/8Bui5BU0", O_CREAT | O_RDWR, 0664);
            ftruncate(shmFd, nr_sh);
            if(shmFd < 0) {
                write(fd_write, &crt_nr, 1);
                write(fd_write, &CRT, crt_nr);
                write(fd_write, &error_nr, 1);
                write(fd_write, &ERROR, error_nr);
                printf("%s\n", ERROR);
                return -1;
            }
            /*
            int* sharedChar = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, 
                MAP_SHARED, nr_sh, 0);
            if(sharedChar == (void*)0) {
                write(fd_write, &ten, 1);
                write(fd_write, &CRT, ten);
                write(fd_write, &five, 1);
                write(fd_write, &ERROR, five);
                printf("%s\n", ERROR);
                return -1;
            }
            */
            write(fd_write, &crt_nr, 1);
            write(fd_write, &CRT, crt_nr);
            write(fd_write, &success_nr, 1);
            write(fd_write, &SUCCESS, success_nr);
        }
        free(p);
        p = NULL;
        return 0;
    }
}