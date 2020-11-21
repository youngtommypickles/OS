#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int res = 0;
    int a;
    int fd = atoi(argv[1]); 

    FILE* file;
    file = fopen(argv[0], "r");

    if(file == NULL){
        printf("Error with opening file\n");
        return 3;
    }
    
    while(!feof(file)){
        fscanf(file, "%d", &a);
        res+=a;
    }

	write(1, &res, sizeof(int));
	fclose(file);
    return 0;
}
