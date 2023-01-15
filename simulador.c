#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM 5
#define SHMSZ 4
#define SHM_ADDR 233

int *param[NUM];
int intervalo, giroscopio1, giroscopio2, nivel_combustible, distancia_inicial, puerto_comunicaciones; // INNGRESO DE DATOS DEL USUARIO
int shmid[NUM];
pthread_t tid, tg1, tg2, tgs;

void *movimiento_cohete(void *arg);
void *sensor_giroscopio1(void *arg);
void *sensor_giroscopio2(void *arg);
void *control_gasolina(void *arg);

void print_help()
/******************************************************************************/
{
    printf("Este programa que permite Simular el viaje de un Cohete\n"
           "uso:\n"
           "Primero Compile el programa usando make\n"
           "Luego de eso ejectue el prgrama con el siguiente comando ./simulador n1, n2. n3, n4, n5\n"
           "Si no lo ingresa tal como está en el paso dos el programa le lanzará esta  alerta y no podrá continuar.\n"
           "\n");
}

int main(int argc, char *argv[])
{
    pthread_attr_t attr;
    if (argc != 7)
    {
        printf("¡Datos ingresados de forma incorrecta!");
        return 0;
    }
    else
    {

        if (nivel_combustible < 0 || nivel_combustible > 100)
        {
            printf("¡El nivel de combustible ingresado es incorrecto!");
            return 0;
        }
        if (inicializar_memoria_compartida() == -1)
            exit(-1);
        intervalo = (int)strtol(argv[1], NULL, 10);
        giroscopio1 = (int)strtol(argv[2], NULL, 10);
        giroscopio2 = (int)strtol(argv[3], NULL, 10);
        nivel_combustible = (int)strtol(argv[4], NULL, 10);
        distancia_inicial = (int)strtol(argv[5], NULL, 10);
        puerto_comunicaciones = (int)strtol(argv[6], NULL, 10);
        // AQUÍ CREACIÓN DE LOS HILOS
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, movimiento_cohete, NULL);
        pthread_create(&tg1, NULL, sensor_giroscopio1, NULL);
        pthread_create(&tg2, NULL, sensor_giroscopio2, NULL);
        // UNIÓN DE LOS HILOS
        pthread_join(tid, NULL);
        pthread_join(tg1, NULL);
        pthread_join(tg2, NULL);

        while (1)
        {
            printf("Valor actual distancia %d, combustible %d, giroscopio 1 %d, girosciopio 2 %d\n", distancia_inicial, nivel_combustible, giroscopio1, giroscopio2);
            if (distancia_inicial == 0)
            {
                break;
            }
        }
    }

    return 0;
}

int inicializar_memoria_compartida(void)
{
    int i;

    for (i = 0; i < NUM; i++)
    {
        if ((shmid[i] = shmget(SHM_ADDR + i, SHMSZ, IPC_CREAT | 0666)) < 0)
        {
            perror("shmget");
            return (-1);
        }
        if ((param[i] = shmat(shmid[i], NULL, 0)) == (int *)-1)
        {
            perror("shmat");
            return (-1);
        }
    }

    distancia_inicial = param[0];
    nivel_combustible = param[1];
    giroscopio1 = param[2];
    giroscopio2 = param[3];
    // alarma = param[4];

    return (1);
}

// Hilo principal
void *movimiento_cohete(void *arg)
{
    while (1)
    {
        usleep(intervalo * 1000);
        distancia_inicial -= 1;

        if (distancia_inicial < 0)
        {
            pthread_exit(0);
        }
        if (distancia_inicial <= 100)
        {
            printf("¡Iniciando secuencia de aterrizaje!\n");
            printf("Encendiendo Propulsor principal\n");
            // Propulsor principal encendido
        }

        if (distancia_inicial < 5 && giroscopio1 < 0 && giroscopio2 < 0)
        {
            distancia_inicial += 30;
        }

        if (distancia_inicial == 1)
        {
            printf("Alunizaje Exitoso!\n");
            // Apagar todos los propulsores
        }
    }
}

// Hilo giroscopio 1
void *sensor_giroscopio1(void *arg)
{
    printf("Giroscopio 1 encendido\n");
    while (1)
    {
        sleep(0.5);
        int correccion = 0;
        if (giroscopio1 > 0)
        {
            printf("Encendiendo Propulsor izquierdo!...\n");
            printf("Enderezando el cohete!\n");
            giroscopio1 = giroscopio1 - correccion;
            correccion += 1;
        }
        else if (giroscopio1 < 0)
        {
            printf("Encendiendo Propulsor derecho!...\n");
            printf("Enderezando el cohete!\n");
            giroscopio1 = giroscopio1 + giroscopio1;
        }
        else
        {
            break;
        }
    }
    printf("Giroscopio 1 apagado\n");
}

// Hilo giroscopio 2
void *sensor_giroscopio2(void *arg)
{
    printf("Giroscopio 2 encendido\n");
    while (1)
    {
        sleep(0.5);
        int correccion = 0;
        if (giroscopio1 > 0)
        {
            printf("Encendiendo Propulsor izquierdo!...\n");
            printf("Enderezando el cohete!\n");
            giroscopio2 = giroscopio2 - correccion;
            correccion += 1;
        }
        else if (giroscopio2 < 0)
        {
            printf("Encendiendo Propulsor derecho!...\n");
            printf("Enderezando el cohete!\n");
            giroscopio2 = giroscopio2 + correccion;
            correccion += 1;
        }
    }
    printf("Giroscopio 2 apagado\n");
}

// Hilo Control Gasolina
void *control_gasolina(void *arg)
{
    // Aquí vamos a controlar la gasolina
    if (nivel_combustible <= 10)
    {
        printf("Gasolina debajo del 10 %%\n!!!!");
        printf("Abortar Aterrizaje!\n");
        // AQUÏ ABORTAR ATERRIZAJE
    };
};