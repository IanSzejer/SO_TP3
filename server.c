#define PORT 8080
#define LEVELS 12
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "levelFunctions.h"

#define MAX_BUFFER_SIZE 1024

typedef struct Level{
    char desafio[2000];
    char respuesta[100];
    char pregunta[2000];
    void (*function)(void);
}Level;

void manageServer();
void openSocket();
static void refresh_screen() ;
static int checkAnswer(char* buffer,int levelNum);
static void printLevel(int level);

int SERVER_FD;
int NEW_SOCKET;

static char* challenge = "------------- DESAFIO -------------\n";
static char* question = "----- PREGUNTA PARA INVESTIGAR -----\n";

static Level levels[LEVELS]={{"Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\n\nEn este TP deberán finalizar el juego que ya comenzaron resolviendo los desafíos de cada nivel.\nAdemás tendrán que investigar otras preguntas para responder durante la defensa.\nEl desafío final consiste en crear un programa que se comporte igual que yo, es decir, que provea los mismos desafíos y que sea necesario hacer lo mismo para resolverlos. No basta con esperar la respuesta.\nAdemás, deberán implementar otro programa para comunicarse conmigo.\n\nDeberán estar atentos a los easter eggs.\n\nPara verificar que sus respuestas tienen el formato correcto respondan a este desafío con la palabra 'entendido\\n' \n", "entendido\n", 
"¿Cómo descubrieron el protocolo, la dirección y el puerto para conectarse?\n",
NULL},
    {"The Wire S1E5\n5295 888 6288\n", "itba\n",
     "¿Qué diferencias hay entre TCP y UDP y en qué casos conviene usar cada uno?\n"
     ,NULL},
    { "https://ibb.co/tc0Hb6w\n\n", "M4GFKZ289aku\n",
     "¿El puerto que usaron para conectarse al server es el mismo que usan para mandar las respuestas? ¿Por qué?\n"
     ,NULL},
    {"EBADF...\n", "fk3wfLCm3QvS\n",
     "¿Qué útil abstracción es utilizada para comunicarse con sockets? ¿Se puede utilizar read(2) y write(2) para operar?\n",
     &level4Func},
    {"respuesta = strings:107\n", "too_easy\n",
     "¿Cómo garantiza TCP que los paquetes llegan en orden y no se pierden?\n",NULL},
    {".data .bss .comment ? .shstrtab .symtab .strtab\n", ".RUN_ME\n",
     "Un servidor suele crear un nuevo proceso o thread para atender las conexiones entrantes. ¿Qué conviene más?\n",NULL},
    {"Filter error\n", "K5n2UFfpFMUN\n",
    "¿Cómo se puede implementar un servidor que atienda muchas conexiones sin usar procesos ni threads?\n",
    &level7Func},
    {"¿?\n", "BUmyYq5XxXGt\n",
     "¿Qué aplicaciones se pueden utilizar para ver el tráfico por la red?\n",
     &level8Func},
    {"Latexme\n\nSi\n\\mathrm{d}y = u^v{\\cdot}(v'{\\cdot}\\ln{(u)}+v{\\cdot}\\frac{u'}{u})\nentonces\ny = \n", "u^v\n", 
    "sockets es un mecanismo de IPC. ¿Qué es más eficiente entre sockets y pipes?\n",NULL}, 
    {"quine\n", "chin_chu_lan_cha\n",                                                                                                                                                         
     "¿Cuáles son las características del protocolo SCTP?\n",&level10Func},
    {"b gdbme y encontrá el valor mágico\nENTER para reintentar.\n", "gdb_rules\n",
     "¿Qué es un RFC?\n",&level11Func},
    {"Me conoces\n", "normal\n", "¿Fue divertido?\n",&level12Func}};

//Esto tira warning, pero lo hacemos para que se cumpla el easterEgg
static char easterEgg[]=" _______________________\n< ESTO ES UN EASTER_EGG >\n-----------------------\n        \\   ^__^\n         \\  (oo)\\_______\n            (__)\\       )\\/\\ \n                ||----w |\n                ||     ||";


int main(int argc, char const *argv[])
{
    openSocket();
    manageServer();
    return 0;
}

void manageServer(){
    int playing=1;
    char buffer[MAX_BUFFER_SIZE];
    int valread;
    int level=0;
    printLevel(level);
    while(playing){
        memset(buffer, 0, MAX_BUFFER_SIZE);
        valread = read(NEW_SOCKET, buffer, 1024);
        if(valread==-1){
            perror("Server: Failed at reading");
            return;
        }
        
        if(checkAnswer(buffer,level)){
            level++;
            if (level<LEVELS){
                printLevel(level);
            }
            else {
                refresh_screen();
                printf("Felicitaciones, finalizaron el juego. Ahora deberán implementar el servidor que se comporte como el servidor provisto\n\n");

                playing=0;
            }
         }
         else{
            printf("Respuesta incorrecta: ");
            printf("%s\n",buffer);
            sleep(2);
            refresh_screen();
            printLevel(level);
        }

         
    }
    // closing the connected socket
    close(NEW_SOCKET);
  // closing the listening socket
    shutdown(SERVER_FD, SHUT_RDWR);
    return;
}

static int checkAnswer(char* buffer,int levelNum){
    return strcmp(buffer,levels[levelNum].respuesta)==0;
} 

static void printLevel(int level){

    refresh_screen();
    printf("%s", challenge);
    printf("%s",levels[level].desafio);
    if(*levels[level].function != NULL){
        (*levels[level].function)();
    }
    printf("%s", question);
    printf("%s",levels[level].pregunta);

}


void openSocket(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
        == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    SERVER_FD = server_fd;
    NEW_SOCKET = new_socket;
    return ;
}


static void refresh_screen() {
    printf("\033[1;1H\033[2J");
}