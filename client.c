#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define SERVER_IP "0.0.0.0"
#define MAX_BUFFER_SIZE 128

static int connectToSocket(struct sockaddr_in server_add);
static int readAndCommunicate(int client_fd);



int main(int argc,char** argv){
    int client_fd;
    struct sockaddr_in serv_addr;
    client_fd= connectToSocket(serv_addr);
    if (client_fd==-1){
        perror("Client: Fallo en la creacion del socket client\n");
        return -1;
    }

    if(readAndCommunicate(client_fd)==-1){
        perror("Client: Fallo en la escritura al server\n");
        return -1;
    }
    close(client_fd);
    return 0;

}


static int connectToSocket(struct sockaddr_in server_add){
    int sock=0;
    int client_fd;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(PORT);

    if(inet_pton(AF_INET, SERVER_IP, &server_add.sin_addr)){
        return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr*)&server_add,sizeof(server_add)))!= 0) {
        
        return -1;
    }

    return client_fd;
}

static int readAndCommunicate(int client_fd){
    char buffer[MAX_BUFFER_SIZE];
    int charsWritten;

    while(fgets(buffer, MAX_BUFFER_SIZE-1,stdin) != NULL){   //Retorna NULL cuando falla
        charsWritten= write(client_fd,buffer,strlen(buffer));
        memset(buffer,0,MAX_BUFFER_SIZE);//Pongo el buffer en 0
        if (charsWritten<0){
            return -1;
        }
    }
    return 0;
}
