#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int PrimeCount(int a, int b){
    int n = 0;
    printf("(The sieve of Eratosthenes");
    int mas[b+1];
    for(int i = 0; i < b+1; ++i){
        mas[i] = i;
    }
    for(int i = 2; i*i <= b; ++i){
        if(mas[i]){
            for(int j = i*i; j <= b; j += i){
                mas[j] = 0;
            }
        }
    }
    for(int i = 0; i <= b; ++i){
        if(mas[i] && i >= a && i != 1){
            n++;
        }
    }
    return n;
}

char* translation(long x){
    printf("(Trenary");
    char* res = (char*)malloc(64 * sizeof(char));
    do{
        *--res = x % 3 + '0';
        x /= 3;
    }
    while (x != 0);
    return res;
}

int main(){
    return 0;
}

