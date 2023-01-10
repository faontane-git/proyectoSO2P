#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000], server_reply[2000];

    // Crea un socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("No se pudo crear el socket");
    }
    puts("Socket creado");

    // Prepara la estructura sockaddr_in
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // dirección IP del servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(8888); // puerto del servidor

    // Conecta al servidor
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Falló la conexión al servidor");
        return 1;
    }

    puts("Conectado al servidor\n");

    // Envía un mensaje al servidor
    printf("Ingresa un mensaje para enviar al servidor: ");
    scanf("%s", message);

    if (send(sock, message, strlen(message), 0) < 0)
    {
        puts("Falló el envío");
        return 1;
    }
    puts("Mensaje enviado\n");

    // Recibe una respuesta del servidor
    if (recv(sock, server_reply, 2000, 0) < 0)
    {
        puts("Falló la recepción");
        return 1;
    }
    puts("Respuesta del servidor:");
    puts(server_reply);

    close(sock);
    return 0;
}
