#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MEMORY_NAME "file"
#define MEMORY_SIZE 4096
#define N 256

typedef struct {
    size_t size;
    int sum[N];
}Data;

int main(int argc, char *argv[]){
    int status;
    char name[N];
    printf("Enter file name: ");
    scanf("%s", name);
    char* arg[] = {name, NULL}; 

    pid_t id = fork();
 

    if (id == 0) {
        execv("./child", arg);
    }else if(id > 0){
        if(waitpid(id, &status, 0) == -1){
            printf("Error wait child process\n");
            return -1;
        }    
        if (WEXITSTATUS(status) != 0) {
            return -2;
        }
        int fd = shm_open(MEMORY_NAME, O_RDONLY, 00600);
        if(fd == -1){
            printf("Error open shared memory file\n");
            return -2;
        }

        Data *data = mmap(NULL, MEMORY_SIZE, PROT_READ, MAP_SHARED, fd, 0);
        if(data == MAP_FAILED){
            printf("Error map\n");
            return -4;
        }

        for (int i = 0; i < data->size; i++) {
            printf("%d Result from child: %d\n", i + 1, data->sum[i]);
        }

        munmap(data, MEMORY_SIZE);
        shm_unlink(MEMORY_NAME);
        close(fd);
    } else{
        printf("Error fork\n");
        return -5;
    }
    return 0;
}
