#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MEMORY_NAME "shm"
#define N 256

int main(int argc, char* argv[]){
    int status;
    int fd = shm_open(MEMORY_NAME, O_RDWR, 0);
    if(fd == -1){
        printf("Error with creating shared memory\n");
        return -1;
    }

    struct stat file_st;
    if(fstat(fd, &file_st) == -1){
        printf("Error fstat\n");
        return -3;
    }

    char* ptr = mmap(NULL, file_st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED){
        printf("Error mmap\n");
        return -3;
    }

    char* arg[] = {ptr, NULL};

    pid_t id = fork();
    if(id == 0){
        execv("./b", arg);
    } else{
        if(waitpid(id, &status, 0) == -1){
            printf("Error wait child process\n");
            return -1;
        }    
        if (WEXITSTATUS(status) != 0) {
            return -2;
        }
        printf("was get by process C\n");
        printf("Result: %s\n", ptr);
        munmap(ptr, file_st.st_size);
        close(fd);
    }
    return 0;
}