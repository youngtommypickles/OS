#include <stdlib.h>
#include <stdio.h>
#include "library1.h"

int main(int argc, char* argv[]) {
    int x, a, b;
    long c;
    for(;;){
        scanf("%d", &x);
        if(x == 1){
            scanf("%d %d", &a, &b);
            printf("Result ");
            int n = PrimeCount(a, b);
            printf("): %d\n", n);
        }else if(x == 2){
            scanf("%ld", &c);
            printf("Result ");
            char* res = translation(c);
            printf("): %s\n", res);
        }else{
            return 0;
        }
    }
}