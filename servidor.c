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

#define NUM 5
#define SHMSZ 4
#define SHM_ADDR 233
int *param[NUM];
int intervalo, giroscopio1, giroscopio2, nivel_combustible, distancia_inicial,
	puerto_comunicaciones; // INGRESO DE DATOS DEL USUARIO
int shmid[NUM];
bool aumentar_potencia = false;
int correccion = 0;
int correccion2 = 0;
pthread_t tid, tg1, tg2;
void *movimiento_cohete(void *arg);
void *sensor_giroscopio1(void *arg);
void *sensor_giroscopio2(void *arg);
void control_gasolina();
int client_sockfd, *num_client, total_client;
bool finish_flag = false;
unsigned short i = 0;
typedef struct
{
	int intervalo;
	int giroscopio1;
	int giroscopio2;
	int nivel_combustible;
	int distancia_inicial;
} datos;
datos ingreso;

/* ............................... *>> REFERENCIAS DE FUNCIONES <<* ................................ */

// void cerrarServidor(int sig);
void desconectarPorSenial(int sig);

int main(int argc, char *argv[])
{
	/* ............................... *>> DECLARACIÓN DE VARIABLES <<* ................................ */
	pthread_attr_t attr;
	int server_sockfd;
	int server_len, opt, port_num;
	int contador;
	unsigned client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int puerto_comunicaciones;
	int semflg = 0666 | IPC_CREAT; /* PERMISOS */
	bool potencia_media = false;
	/* ............................. *>> CREAR SOCKET PARA EL SERVIDOR <<* ............................. */

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);
	server_address.sin_port = htons(7734);
	server_len = sizeof(server_address);

	/* ......................... *>> TOMAR ARGUMENTOS ENVIADOS POR CONSOLA <<* ......................... */

	printf("Este es el servidor de DarasViendo.\nEjecute el cliente para poder conectarse al servidor.\n");
	puerto_comunicaciones = atoi(argv[1]);
	server_address.sin_port = htons(puerto_comunicaciones);

	/* ............................. *>> CREAR CONEXIÓN PARA EL SERVIDOR <<* ........................... */

	if (bind(server_sockfd, (struct sockaddr *)&server_address, server_len) == -1)
	{
		perror("NO SE PUDO CONECTAR PUES FALLO EL BIND\n");
		exit(1);
	}
	if (listen(server_sockfd, 2) == -1)
	{
		perror("NO SE PUDO CONECTAR PUES FALLO EL LISTEN\n");
		exit(1);
	}

	// signal(SIGINT, cerrarServidor);

	/* ................................... *>> ACEPTANDO CLIENTE <<* ................................... */

	do
	{
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

		if (read(client_sockfd, &ingreso, sizeof(ingreso)) == 1)
		{
			perror("Error: receiving data\n");
			exit(1);
		}

		// AQUÍ CREACIÓN DE LOS HILOS
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, movimiento_cohete, NULL);
		pthread_create(&tg1, NULL, sensor_giroscopio1, NULL);
		pthread_create(&tg2, NULL, sensor_giroscopio2, NULL);
		sleep(0.5);
		// UNIÓN DE LOS HILOS
		pthread_join(tid, NULL);
		pthread_join(tg1, NULL);
		pthread_join(tg2, NULL);
		printf("Hilos Terminados\n");

	} while (1);
	return 0;
}

// Hilo principal
void *movimiento_cohete(void *arg)
{
	while (1)
	{
		usleep(ingreso.intervalo * 1000);
		ingreso.distancia_inicial -= 1;
		if (aumentar_potencia)
		{
			ingreso.nivel_combustible -= 5;
		}
		else
		{
			ingreso.nivel_combustible -= 1;
		}

		tablero_informacion();
		if (ingreso.distancia_inicial < 0)
		{
			pthread_exit(0);
		}
		if(ingreso.distancia_inicial>0 && (ingreso.giroscopio1 == 0 && ingreso.giroscopio2 == 0)){
			aumentar_potencia=true;
		}
		if (ingreso.distancia_inicial <= 100 && (ingreso.giroscopio1 == 0 && ingreso.giroscopio2 == 0))
		{
			printf("¡Iniciando secuencia de aterrizaje!\n");
			printf("Encendiendo Propulsor principal\n");
			// Propulsor principal a máxima potencia
			printf("Propulsor a máxima potencia!!!!!!");
			aumentar_potencia = true;
		}
		if (ingreso.distancia_inicial <= 100 && (ingreso.giroscopio1 > 0 || ingreso.giroscopio2 > 0))
		{
			printf("¡Iniciando secuencia de aterrizaje!\n");
			printf("Encendiendo Propulsor principal\n");
			// Propulsor principal a máxima potencia
			aumentar_potencia = false;
		}

		if (ingreso.distancia_inicial < 5 && (ingreso.giroscopio1 > 0 || ingreso.giroscopio2 > 0))
		{
			ingreso.distancia_inicial += 30;
			printf("Secuencia de aterriaje cancelada!!!\n");
			printf("¡El cohete no está orientado para aterrizar y la distancia es de menos de 5m\n");
			printf("Reiniciando secuencia de aterrizaje!!!\n");
		}

		if (ingreso.distancia_inicial == 1)
		{
			printf("Apgando todos los propulsores!...");
			printf("Alunizaje Exitoso!\n");
			ingreso.distancia_inicial = 0;
			// Apagar todos los propulsores
			pthread_exit(0);
			break;
		}
	}
	pthread_exit(0);
}

// Hilo giroscopio 1
void *sensor_giroscopio1(void *arg)
{
	printf("Giroscopio 1 encendido\n");
	while (1)
	{
		sleep(1);
		if (ingreso.giroscopio1 > 0 && ingreso.nivel_combustible >= 0)
		{
			printf("Encendiendo Propulsor izquierdo!...\n");
			printf("Enderezando el cohete!\n");
			printf("GIROSCOPIO 1: %d\n", ingreso.giroscopio1);
			ingreso.giroscopio1 = ingreso.giroscopio1 - 0.5;
		}
		else if (ingreso.giroscopio1 < 0 && ingreso.nivel_combustible >= 0)
		{
			printf("Encendiendo Propulsor derecho!...\n");
			printf("Enderezando el cohete!\n");
			ingreso.giroscopio1 = ingreso.giroscopio1 + 0.5;
		}
		else
		{
			break;
		}
	}
	printf("Giroscopio 1 apagado\n");
	pthread_exit(0);
}

// Hilo giroscopio 2
void *sensor_giroscopio2(void *arg)
{
	printf("Giroscopio 2 encendido\n");
	while (1)
	{
		sleep(1);
		if (ingreso.giroscopio2 > 0 && ingreso.nivel_combustible >= 0)
		{
			printf("Encendiendo Propulsor izquierdo!...\n");
			printf("Enderezando el cohete!\n");
			printf("GIROSCOPIO 2: %d\n", giroscopio2);
			ingreso.giroscopio2 = ingreso.giroscopio2 - 0.5;
		}
		else if (ingreso.giroscopio2 < 0 && ingreso.nivel_combustible >= 0)
		{
			printf("Encendiendo Propulsor derecho!...\n");
			printf("Enderezando el cohete!\n");
			ingreso.giroscopio2 = ingreso.giroscopio2 + 0.5;
		}
		else
		{
			break;
		}
	}
	printf("Giroscopio 2 apagado\n");
	pthread_exit(0);
}

void tablero_informacion()
{
	printf("Valor actual distancia %d, combustible %d, giroscopio 1 %d, "
		   "girosciopio 2 %d\n",
		   ingreso.distancia_inicial, ingreso.nivel_combustible, ingreso.giroscopio1, ingreso.giroscopio2);
}