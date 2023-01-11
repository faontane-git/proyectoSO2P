#include <stdio.h>
#include <pthread.h>

#define NUM 5

int *param[NUM], intervalo, giroscopio1, giroscopio2, nivel_combustible, distancia_inicial, puerto_comunicaciones;
pthread_t tid, tg1, tg2;
#define NUM 5
void *movimiento_cohete(void *arg);
void *sensor_giroscopio1(void *arg);
void *sensor_giroscopio2(void *arg);


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
    intervalo = (int)strtol(argv[1], NULL, 10);
    giroscopio1 = (int)strtol(argv[2], NULL, 10);
    giroscopio2 = (int)strtol(argv[3], NULL, 10);
    nivel_combustible = (int)strtol(argv[4], NULL, 10);
    distancia_inicial = (int)strtol(argv[5], NULL, 10);
    puerto_comunicaciones = (int)strtol(argv[6], NULL, 10);
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

        // AQUÍ TRABAJAR
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, movimiento_cohete, NULL);

        while (1)
        {
            printf("Valor actual distancia %d, combustible %d\n", distancia_inicial, nivel_combustible);
            if (distancia_inicial == 0)
            {
                break;
            }
        }
    }

    return 0;
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
            printf("¡Iniciando secuencia de aterrizaje!");
            printf("Encendiendo Propulsor principal");
            // Propulsor principal encendido
        }

        if (distancia_inicial == 1)
        {
            printf("Alunizaje Exitoso!");
        }
    }
}

void *sensor_giroscopio1(void *arg)
{

    if (giroscopio1 > 0)
    {
        printf("Enderezando el cohete!");
        giroscopio1 = giroscopio1 - giroscopio1;
    }
    else if (giroscopio1 < 0)
    {
        printf("Enderezando el cohete!");
        giroscopio1 = giroscopio1 + giroscopio1;
    }
}

void *sensor_giroscopio2(void *arg)
{

    if (giroscopio1 > 0)
    {
        printf("Enderezando el cohete!");
        giroscopio2 = giroscopio2 - giroscopio2;
    }
    else if (giroscopio2 < 0)
    {
        printf("Enderezando el cohete!");
        giroscopio2 = giroscopio2 + giroscopio2;
    }
}