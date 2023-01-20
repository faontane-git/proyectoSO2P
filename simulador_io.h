# ifndef _PGMA_IO_H
# define _PGMA_IO_H
/* ..................................... *>> INCLUDES <<* .......................................... */
       # include <arpa/inet.h>
       # include <ctype.h>
       # include <dirent.h>
       # include <errno.h>
       # include <fcntl.h>
       # include <getopt.h>
       # include <math.h>
       # include <netinet/in.h>
       # include <pthread.h>
       # include <regex.h>
       # include <semaphore.h>
       # include <signal.h>
       # include <stdio.h>
       # include <stdbool.h>
       # include <stdlib.h>
       # include <string.h>
       # include <sys/ipc.h>
       # include <sys/sem.h>
       # include <sys/shm.h>
       # include <sys/socket.h>
       # include <sys/stat.h>
       # include <sys/sysinfo.h>
       # include <sys/time.h>
       # include <sys/types.h>
       # include <sys/wait.h>
       # include <sys/queue.h>
       # include <unistd.h>

# define MAXBUF 4000
# define IMG_FILE "./archivos/Lista_de_imagenes.txt"
# define MAXH 25

/* ............................... *>> ENCABEZADO DE FUNCIONES <<* ................................. */

       int separar_tokens(char *linea, char *delim, char *buf[]);
       void print_help(char *arg);

       void *obtenerDatoCompartido(key_t key, int flag);
       void desvincularDatoCompartido(const void *puntero);
       void removerDatoCompartido(key_t key, int flag);
       void iniciarSem(key_t key, int flag, int *value, int attr_value);
       void controlSemaforo(int sem, struct sembuf asem[], int value);

/* ................................. *>> FUNCIONES DE MAPA <<* ..................................... */

       Map *newMap(int size);
       void createKey(pgm *key, Map * mapa);
       MapValue *insertMapValue(block *data, Map * mapa);
       int isComplete(pgm *key, Map * mapa);

/* .................. *>> FUNCIÓN PARA CAMBIAR EL EFECTO DE LA IMAGEN PGM <<* ...................... */

       int *pgma_sharpen (block *b, const int filtro[3][3]);

/* ................................ *>> FUNCIONES E/S PGM <<* ...................................... */

       int i4_min ( int i1, int i2 );
       int i4mat_max ( int m, int n, int a[] );

       void pgma_check_data ( int xsize, int ysize, int maxg, int *garray );
       void pgma_example ( int xsize, int ysize, int *garray );

       int pgma_read ( char *file_in_name, int *xsize, int *ysize, int *maxg,
              int **garrary );
       void pgma_read_data ( FILE *file_in, int xsize, int ysize, int *garray );
       void pgma_read_header ( FILE *file_in, int *xsize, int *ysize, int *maxg );


       void pgma_write ( char *file_name, char *comment, int xsize, int ysize, 
       int maxval, int *gray );
       void pgma_write_old ( char *file_out_name, int xsize, int ysize, int *garray );
       void pgma_write_data ( FILE *file_out, int xsize, int ysize, int *garray );
       void pgma_write_header ( FILE *file_out, char *file_out_name, int xsize, 
              int ysize, int maxg );

       void test_pgma_read ( char *file_name );
       void test_pgma_write ( char *file_name );

# endif