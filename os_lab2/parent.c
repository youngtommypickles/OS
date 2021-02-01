#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 255

#define ERROR_PIPE -1
#define ERROR_OPENING -2
#define ERROR_DUP2 -3

int main(int argc, char* argv[]){
    int fd[2];
    char name[N];
    printf("Enter name of file: ");
    scanf("%s", name);

    if(pipe(fd) == -1){
		printf("Error with opening pipe\n");
		return ERROR_PIPE;
	}

    if(fork() > 0) {
        close(fd[1]);
        int n = 1;
        int res;
        while(read(fd[0], &res, sizeof(int))){
            printf("%d Result from child:%d\n", n, res);
            n++;
        }
        close(fd[0]);
    }else {
        if (dup2(fd[1],1) == -1){
            printf("Error with opening dup2\n");
            return ERROR_DUP2;
        }
        char* arg[] = {name, NULL}; 
        execv("./child", arg);
    }
    return 0;
}