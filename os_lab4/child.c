#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MEMORY_NAME "file"
#define MEMORY_SIZE 4096
#define N 256

typedef struct {
    size_t size;
    int sum[N];
}Data;

void func(FILE *input,Data *data) {
    char c;
	int a, b, res, flag;
    a = 0; res =0; flag = 0;
    data->size = 0;
	for(;;) {
        c = fgetc(input);

        if (c >= '0' && c <= '9') {
            b = atoi(&c);
            a = a*10 + b;
		}else if (c == '-') {
			flag = 1;
		}else if(c == ' ') {
			if(flag){
                res-=a;
                flag = 0;
            }else{
                res+=a;
            }
            a = 0;
		}else if(c == '\n') {
			if(flag){
                res-=a;
                flag = 0;
            }else{
                res+=a;
            }
            data->sum[data->size++] = res;
            res = 0;
            a = 0;
		}else if(c == EOF){
            return;
        }
	}
}

int main(int argc, char *argv[]) {
    int fd = shm_open(MEMORY_NAME, O_EXCL | O_CREAT | O_RDWR, 00600);
    if(fd < 0){
        printf("Error sh_open\n");
        return -2;
    }
    if(ftruncate(fd, MEMORY_SIZE)){
        printf("Error with length of file\n");
        return -3;
    }

    Data* data = mmap(NULL, MEMORY_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if(data == MAP_FAILED){
        printf("Error map\n");
        return -4;
    }
    FILE* input = fopen(argv[0], "r");
    if(input ==  NULL){
        printf("Error open file\n");
        return -5;
    }
    func(input, data);
    fclose(input);
    munmap(data, MEMORY_SIZE);
    close(fd);
    return 0;
}
