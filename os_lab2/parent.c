#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int fd[2];
    char name[100];
    printf("Enter name of file: ");
    scanf("%s", name);

    if(pipe(fd) == -1){
		printf("Error with opening pipe\n");
		return 2;
	}

    char fd_ = fd[1] + '0';
    char* arg[] = {name, &fd_, NULL};

    if (fork() > 0) {
        int res;
        read(fd[0], &res, sizeof(int)); 
        printf("Result from child:%d\n", res);
    }else  {
        if (dup2(fd[1],1) == -1){
            printf("Error with opening dup2\n");
            return 1;
        }
        if (dup2(fd[0],0) == -1){
            printf("Error with opening dup2\n");
            return 1;
        }
        execve("./child", arg, NULL);
    }
  return 0;
}