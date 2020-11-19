#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int i = 0;
    int fd = atoi(argv[1]); //Для перевода дескрипторов из char* обратно в int

    FILE* file;
    file = fopen(argv[0], "r");

    if(file == NULL){
        printf("Error with opening file\n");
        return 3;
    }

    int a1, a2, a3;
    fscanf(file, "%d %d %d\n", &a1, &a2, &a3);
	int res = a1 + a2 + a3;

	write(1, &res, sizeof(int));
	fclose(file);
    return 0;
}
