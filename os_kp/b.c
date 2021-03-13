#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    int n = strlen(argv[0]);
    printf("%d symbols ", --n);
    return 0;
}
