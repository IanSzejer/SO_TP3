#include "levelFunctions.h"

static void gdbme();
static double getRandomNormalvalue();

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


void level8Func(){
    printf("\033[30;40mLa respuesta es BUmyYq5XxXGt\033[0m\n\n");
}

//Ejecuto el quino.c
void level10Func(){
    int gcc_quine=system("gcc quine.c -o quine");

    if(gcc_quine == 0){
        printf("¡Genial!, ya lograron meter un programa en quine.c, veamos si hace lo que corresponde.\n");
        int quine_diff=system("./quine | diff - quine.c");
        if (quine_diff == 0) {
            printf("\nLa respuesta es chin_chu_lan_cha\n\n");
        } else {
            printf("\ndiff encontró diferencias.\n");
        }
    }else{
        printf("\n\nENTER para reintentar\n\n");
    }
}

void level11Func(){
    gdbme();
}

void level12Func(){
    for (int i = 0; i < MAX; i++) {
        printf("%.6f ", getRandomNormalvalue());
    }
    printf("\n");
}

static void gdbme() {
    if (getpid() == GDB_MAGIC_NUMBER) {
        printf("La respuesta es gdb_rules\n");
    }
}

static double getRandomNormalvalue(){
    return sqrt(-2 * log((double)rand()/RAND_MAX + 1.0)) * cos(2*M_PI* ((double)rand()/RAND_MAX + 1.0));
}
