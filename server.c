
#define PORT 8080
#define LEVELS 12
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAX_BUFFER_SIZE 1024
typedef struct Level{
    char desafio[200];
    char pregunta[200];
    char respuesta[100];
    void (*function)(void)
}Level;

static int checkAnswer(char* buffer,int levelNum);
static void printLevel(int level);

static Level levels[LEVELS]={{"Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\n\nEn este TP deberán finalizar el juego que ya comenzaron resolviendo los desafíos de cada nivel.\nAdemás tendrán que investigar otras preguntas para responder durante la defensa.\nEl desafío final consiste en crear un programa que se comporte igual que yo, es decir, que provea los mismos desafíos y que sea necesario hacer lo mismo para resolverlos. No basta con esperar la respuesta.\nAdemás, deberán implementar otro programa para comunicarse conmigo.\n\nDeberán estar atentos a los easter eggs.\n\nPara verificar que sus respuestas tienen el formato correcto respondan a este desafío con la palabra 'entendido\\n'", "entendido\n", 
"¿Cómo descubrieron el protocolo, la dirección y el puerto para conectarse?",
NULL},
    {"The Wire S1E5\n5295 888 6288", "itba\n",
     "¿Qué diferencias hay entre TCP y UDP y en qué casos conviene usar cada uno?"
     ,NULL},
    { "https://ibb.co/tc0Hb6w\n\n", "M4GFKZ289aku\n",
     "¿El puerto que usaron para conectarse al server es el mismo que usan para mandar las respuestas? ¿Por qué?"
     ,NULL},
    {"EBADF...\n", "fk3wfLCm3QvS\n",
     "¿Qué útil abstracción es utilizada para comunicarse con sockets? ¿Se puede utilizar read(2) y write(2) para operar?",
     &level4Func},
    {"respuesta = strings:no se sabe", "too_easy\n",
     "¿Cómo garantiza TCP que los paquetes llegan en orden y no se pierden?",NULL},
    {".data .bss .comment ? .shstrtab .symtab .strtab", ".RUN_ME\n",
     "Un servidor suele crear un nuevo proceso o thread para atender las conexiones entrantes. ¿Qué conviene más?",NULL},
    {"Filter error", "K5n2UFfpFMUN\n",
     "¿Cómo se puede implementar un servidor que atienda muchas conexiones sin usar procesos ni threads?"
     &level7Func},
    {"¿?", "BUmyYq5XxXGt\n",
     "¿Qué aplicaciones se pueden utilizar para ver el tráfico por la red?"
     &level8Func},
    {"Latexme\n\nSi\n\\mathrm{d}y = u^v{\\cdot}(v'{\\cdot}\\ln{(u)}+v{\\cdot}\\frac{u'}{u})\nentonces\ny =", "u^v\n", 
    "sockets es un mecanismo de IPC. ¿Qué es más eficiente entre sockets y pipes?",NULL}, 
    {"quine", "chin_chu_lan_cha\n",                                                                                                                                                         
     "¿Cuáles son las características del protocolo SCTP?",&level10Func},
    {"b gdbme y encontrá el valor mágico\nENTER para reintentar.", "gdb_rules\n",
     "¿Qué es un RFC?",&level11Func},
    {"Me conoces", "normal\n", "¿Fue divertido?",&level12Func}};

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
    while(1){
        valread = read(new_socket, buffer, 1024);
        if(valread==-1){
            perror("Server: Failed at reading");
            return;
        }
        printf("%s\n",buffer);
        if(checkAnswer(buffer,level)){
            level++;
            if (level<LEVELS){
                printLevel(level);}
            else {
                printf("Felicitaciones, finalizaron el juego. Ahora deberán implementar el servidor que se comporte como el servidor provisto\n\n");

                playing=0;
            }
         }
         else{
            printf("Respuesta incorrecta");
            sleep(2);
            printLevel(level);}

         
    }
    // closing the connected socket
    close(new_socket);
  // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}

static int checkAnswer(char* buffer,int levelNum){
    return strcmp(buffer,level[levelNum].respuesta)==0;
} 

static void printLevel(int level){
    clrscr();
    printf("%s",levels[level].desafio);
    (*levels[level].function)();
    printf("%s",levels[level].pregunta)
}


int openSocket(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
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
    
}
