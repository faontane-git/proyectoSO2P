#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <math.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/queue.h>
#include <unistd.h>
#include <ctype.h>
#define MAXBUF 4000

int server_pid, sockfd;
int *hilos;
int puerto_comunicaciones;
typedef struct
{
	int intervalo;
	int giroscopio1;
	int giroscopio2;
	int nivel_combustible;
	int distancia_inicial;
} datos;
/* ............................... *>> REFERENCIAS DE FUNCIONES <<* ................................ */
void desconectarPorSenial(int sign);
void cerrarConexion(int sign);
char enviarMensaje(char *arv);
void *obtenerDatoCompartido(key_t key, int flag);

int main(int argc, char *argv[])
{
	/* ............................... *>> DECLARACIÓN DE VARIABLES <<* ................................ */

	int status, opt, port_num, reg_value, reg_out;
	char commandFile[MAXBUF];
	struct sockaddr_in ip;
	int contador;
	datos ingreso;

	ip.sin_family = AF_INET;
	ip.sin_addr.s_addr = inet_addr("127.0.0.1");
	ip.sin_port = htons(7734);
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("Socket create failed.\n");
		return -1;
	}

	/* ......................... *>> TOMAR ARGUMENTOS ENVIADOS POR CONSOLA Y VALIDAR <<* ......................... */
	ingreso.intervalo = atoi(argv[1]);
	ingreso.giroscopio1 = atoi(argv[2]);
	ingreso.giroscopio2 = atoi(argv[3]);
	ingreso.nivel_combustible = atoi(argv[4]);
	ingreso.distancia_inicial = atoi(argv[5]);
	puerto_comunicaciones = atoi(argv[6]);
 	ip.sin_port = htons(puerto_comunicaciones);
	
	
	ingreso.intervalo = atoi(argv[1]);
	// Validar el ingreso del ángulo 1
	// Validar que sea entero
	if (ingreso.giroscopio1 < -91 || ingreso.giroscopio1> 90)
	{
		printf("El ángulo 1 no debe de ser mayor de 90 ni menor a -91");
		return -1;
	}
	
	// Validar el ingreso del ángulo 2
	// Validar que sea entero
	if (ingreso.giroscopio2 < -91 || ingreso.giroscopio2 > 90)
	{
		printf("El ángulo 2 no debe de ser mayor de 90 ni menor a -91");
		return -1;
	}
	// Validar el nivel de combustible

	if (ingreso.nivel_combustible <= 0 || ingreso.nivel_combustible> 100)
	{
		if (ingreso.nivel_combustible== 0)
		{
			printf("Flaco no puede ingresar nada de combustilble a la nave ponte pilas, ¡Vuelve a ingresar los datos!\n");
		}
		if (ingreso.nivel_combustible< 0)
		{
			printf("Flaco no puede ingresar combustilble negativo a la nave ponte pilas, ¡Vuelve a ingresar los datos!\n");
		}
		if (ingreso.nivel_combustible> 100)
		{
			printf("Flaco no puede ingresar más combustilble de lo permitido a la nave ponte pilas, ¡Vuelve a ingresar los datos!\n");
		}
		return (-1);
	}
	

	/* ............................... *>> CONEXIÓN CON EL SERVIDOR <<* ................................ */

	int result = connect(sockfd, (struct sockaddr *)&ip, sizeof(ip));
	if (result == -1)
	{
		perror("Error has occurred");
		exit(-1);
	}

	if (send(sockfd, &ingreso, sizeof(ingreso), 0) == -1)
	{
		perror("Error: sending two values to server");
	}

	/* ............................. *>> COMANDOS DEL CLIENTE <<* ............................... */

	printf("Este es la consola cliente\n");

	while (1)
	{
		printf("Ingrese un comando: ");
		fgets(commandFile, MAXBUF, stdin);
		strtok(commandFile, "\n");
		break;
	}
	close(sockfd);

	return 0;
}

/* ************************************************************************************************* */
/* 								      FUNCIONES - MANEJO DE SEÑALES 								 */
/* ************************************************************************************************* */

void desconectarPorSenial(int sign)
/**
 * Propósito:
 * 		Se desconecta del SERVIDOR enviando una señal y termina el proceso CLIENTE.
 * Parámetros:
 *		Entrada, int SIG, el número de la señal hacia este proceso.
 */
{
	printf("\nDesconectando del servidor");
	kill(server_pid, SIGCONT);

	close(sockfd);
	printf("\n[!!] Servidor desconectado\nBYE!\n");
	exit(0);
}

void cerrarConexion(int sign)
/**
	close(sockfd);
 * Propósito:
 * 		Se desconecta del SERVIDOR recibiendo una señal y termina el proceso CLIENTE.
 *
 * Parámetros:
 *		Entrada, int SIG, el número de la señal hacia este proceso.
 */
{
	printf("\n[!!] Cliente desconectado por desconexión de servidor\n");
	close(sockfd);
	exit(0);
}
