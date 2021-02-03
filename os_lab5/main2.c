#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <math.h>

#define LIBRARY1 "./library1.so"
#define LIBRARY2 "./library2.so"

int main(int argc, char* argv[]) {
    void *library;
    int x, a, b;
    long c;

    if(argv[1]){
        x = atoi(argv[1]);
    }

    if(x == 0){
        library = dlopen(LIBRARY2, RTLD_LAZY);
    } else{
        library = dlopen(LIBRARY1, RTLD_LAZY);
    }

    if (!library) {
        printf("Error dlopen(): %s\n", dlerror());
        return 1;
    }

    int(*PrimeCount)(int x, int y) = dlsym(library, "PrimeCount");
    char*(*translation)(long x) = dlsym(library, "translation");

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
            dlclose(library);
            return 0;
        }
    }
}