#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#define NORMALISE 4
#define FIRST_LETTER 32
#define LAST_LETTER 126

void level4Func() {
    if (write(13, "................................La respuesta es fk3wfLCm3QvS\n", 61) == -1) {
        perror("write");
    }
}

void level7Func(){
    
    char* answer = "La respuesta es K5n2UFfpFMUN\n";
    int randomfd;

    int i = 0;
    while (answer[i] != 0) {
        randomfd = (rand() % NORMALISE) + 1;

        if (randomfd == STDOUT_FILENO ) {
            write(STDOUT_FILENO, answer + i, 1);
            i++;
        }
        else {
            char c = (char)(rand() % (FIRST_LETTER - LAST_LETTER + 1) + FIRST_LETTER);
            write(STDERR_FILENO, &c, 1);
        }
    }
}
