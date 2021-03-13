#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define MEMORY_NAME "shm"
#define N 256

int main(int argc, char* argv[]){
    int status;
    char str[N];
    printf("write 0 to stop programm\n");
    for(;;){
        printf("Enter string: ");
        fgets(str, sizeof(str), stdin);
        if(str[0] == '0'){
            return 0;
        }
        char* arg1[] = {NULL};
        char* arg2[] = {str, NULL};

        int fd = shm_open(MEMORY_NAME, O_CREAT | O_RDWR, 00600);
        if(fd == -1){
            printf("Error with creating shared memory\n");
            return -1;
        }

        if(ftruncate(fd, N) == -1){
            printf("Error with rising\n");
            return -2;
        }

        char* ptr = mmap(0, sizeof(str), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(ptr == MAP_FAILED){
            printf("1. Error mmap\n");
            return -3;
        }
        memcpy(ptr, str, sizeof(str));
        munmap(ptr, sizeof(str));
        close(fd);

        pid_t id = fork();

        if(id == 0){
            pid_t ch_id = fork();
            if(ch_id == 0){
                execv("./b", arg2);
            } else{
                if(waitpid(ch_id, &status, 0) == -1){
                    printf("Error wait child process\n");
                    return -1;
                }    
                if (WEXITSTATUS(status) != 0) {
                    return -2;
                }
                printf("was sent by process A\n");
                execv("./c", arg1);
            }
        }else{
            if(waitpid(id, &status, 0) == -1){
                printf("Error wait child process\n");
                return -1;
            }    
            if (WEXITSTATUS(status) != 0) {
                return -2;
            }
            shm_unlink(MEMORY_NAME);
        }
    }
        
        
    return 0;
}
