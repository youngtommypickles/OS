#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <math.h>
#include <stdbool.h>

#define LIBRARY1 "./library1.so"
#define LIBRARY2 "./library2.so"

int main(int argc, char* argv[]) {
    void *library;
    bool type = true;
    int x, a, b;
    long c;

    library = dlopen(LIBRARY1, RTLD_LAZY);
    if (!library) {
        printf("Error dlopen(): %s\n", dlerror());
        return 1;
    }

    int(*PrimeCount)(int x, int y);
    char*(*translation)(long x);
    *(void**) (&PrimeCount) = dlsym(library, "PrimeCount");
    *(void**) (&translation) = dlsym(library, "translation");

    for(;;){
        scanf("%d", &x);
        if(x == 0){
            dlclose(library);
            if(type){
                library = dlopen(LIBRARY2, RTLD_LAZY);
                type = false;
            } else{
                library = dlopen(LIBRARY1, RTLD_LAZY);
                type = true;
            }
            if (!library) {
                printf("Error dlopen(): %s\n", dlerror());
                return 1;
            }
            *(void**) (&PrimeCount) = dlsym(library, "PrimeCount");
            *(void**) (&translation) = dlsym(library, "translation");
        }
        else if(x == 1){
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
