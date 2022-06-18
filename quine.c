#include <stdio.h>

int main(void){
    char c;
    FILE *myFile;
    myFile = fopen("quine.c", "r");
    while((c=fgetc(myFile)) != EOF){
        printf("%c",c);
    }
    fclose(myFile);
    return 0;
}