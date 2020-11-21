#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    char c;
	int a, b, res, flag;
    a = 0; res =0; flag = 0;
	while (scanf("%c", &c) > 0) {
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
		}
	}
    return 0;
}