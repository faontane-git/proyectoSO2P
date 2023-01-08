#include <stdio.h>

void print_help()
/******************************************************************************/
{
    printf("Este programa que permite crear y monitoriar los pedidos de una emperesa\n"
           "uso:\n"
           "Primero Compile el programa usando make\n"
           "Luego de eso ejectue el prgrama con el siguiente comando ./chasquiEats n1, n2. n3, n4, n5\n"
           "Donde n1 es el número de tamaño de la grilla, n2 un número que respresenta los resturantes\n"
           "disponibles en la grilla, n3 el intervalo en valor de milisegundos para ciertos procesos, n4 la cantidad\n"
           "de motorizados disponibles y n5 la cantidad de kilómetros de distancia.\n"
           "Si no lo ingresa tal como está en el paso dos el programa le lanzará esta  alerta y no podrá continuar.\n"
           "\n");
}

int main(int argc, char *argv[])
{
    int intervalo = (int)strtol(argv[1], NULL, 10);
    int giroscopio1=(int)strtol(argv[2],NULL,10);
    int giroscopio2=(int)strtol(argv[3],NULL,10);
    int nivel_combustible=(int)strtol(argv[4],NULL,10);
    int distancia_inicial=(int)strtol(argv[5],NULL,10);
    int puerto_comunicaciones=(int)strtol(argv[6],NULL,10);
    if (argc != 7)  
    { 
        printf("¡Datos ingresados de forma incorrecta!"); 
        return 0; 
    } 
    else{
        if(nivel_combustible<0 || nivel_combustible>100){
            printf("¡El nivel de combustible ingresado es incorrecto!");
            return 0;
        }
        else{
            //AQUÏ TRABAJAR
        }
    }
  

    return 0;
}