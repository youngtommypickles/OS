#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR_FOPEN -4

void func(FILE *input) {
    char c;
	int a, b, res, flag;
    a = 0; res =0; flag = 0;
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
            write(1, &res, sizeof(res));
            res = 0;
            a = 0;
		}else if(c == EOF){
            return;
        }
	}
}

int main(int argc, char* argv[]){
    FILE* input = fopen(argv[0], "r");
    if(input ==  NULL){
        printf("Error open file\n");
        return ERROR_FOPEN;
    }
    func(input);
    return 0;
}