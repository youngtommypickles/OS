#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 255

int main(int argc, char* argv[]){
    int fd[2], id;
    char name[N];
    printf("Enter name of file: ");
    scanf("%s", name);

    if(pipe(fd) == -1){
		printf("Error with opening pipe\n");
		return -1;
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
        FILE* file = freopen(name, "r", stdin);
        if (dup2(fd[1],1) == -1){
            printf("Error with opening dup2\n");
            return -3;
        }
        char** arg = NULL;
        execve("./child", arg, NULL);
    }
    return 0;
}