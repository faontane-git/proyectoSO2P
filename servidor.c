// Servidor
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];

    // Crea un socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("No se pudo crear el socket");
    }
    puts("Socket creado");

    // Prepara la estructura sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Vincula el socket a una dirección y puerto específicos
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        // imprime un mensaje de error y cierra el programa
        perror("Falló la vinculación");
        return 1;
    }
    puts("Vinculación completada");

    // Escucha las solicitudes de
    return 0;
}