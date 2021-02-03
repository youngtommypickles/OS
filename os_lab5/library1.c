#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

bool prime(int n){
    if(n == 0 || n == 1){
        return false;
    }
    int b = sqrt(n);
    for(int i = 2; i <= b; ++i){
        if(n%i == 0){
            return false;
        }
    }
    return true;
}

int PrimeCount(int a, int b){
    int n = 0;
    printf("(Usual way");
    for(int i = a; i < b + 1; ++i){
        if(prime(i)){
            n++;
        }
    }
    return n;
}

char* translation(long x){
    printf("(Binary");
    char* res = (char*)malloc(64 * sizeof(char));
    do{
        *--res = x % 2 + '0';
        x /= 2;
    }
    while (x != 0);
    return res;
}

int main(){
    return 0;
}