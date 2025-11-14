/******************************************************
* Fecha: 11/11/2025
* Pontificia Universidad Javeriana
* Profesor: J. Corredor, PhD
* Autor(es): Alejandro Beltran, Mauricio Beltran & Andres Diaz
* Materia: Sistemas Operativos
* Tema: Taller 03 - Sincronizacion con Hilos (Actividad 2)
******************************************************/
#define MAX_BUFFERS 10 //Espacio maximo que tiene el buffer

#include <pthread.h> // Libreria para hilos y posix
#include <stdio.h> // Libreria para funciones de entrada y salida
#include <stdlib.h> // Libreria para manejo de memoria y exit
#include <string.h> //Libreria para cadenas de caracteres
#include <unistd.h> // Libreria para sleep y close


char buf[MAX_BUFFERS][100]; //Tamano para el buffer que almacena lo del productor
int buffer_index = 0; //Indice del buffer (escritura)
int buffer_print_index = 0; //Indice del buffer (lectura)

pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER; //Mutex para evitar conflictos
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER; //Indicador de que hay espacio
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER; //Indicador de que hay datos
int buffers_available = MAX_BUFFERS; //Que todavia hay espacio para escribir
int lines_to_print = 0; //Contador de mensajes que falta mostrar

void *producer(void *arg);
void *spooler(void *arg);

int main(int argc, char **argv) {
    pthread_t tid_producer[10], tid_spooler; //ID de los hilos
    int i, r; //Variables de apoyo
    //Hilo encargado de mostrar lo producido, sino logra crearlo muestra error y lo termina
    if ((r = pthread_create(&tid_spooler, NULL, spooler, NULL)) != 0) {
        fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); exit(1);
    }
    //Crea los 11 hilos productores
    int thread_no[10]; //Tiene los ID de los productores
    for (i = 0; i < 10; i++) { //Bucle de 11 iteraciones
        thread_no[i] = i; //Asigna el ID a cada hilo, si no lo logra muestra error y lo termina
        if ((r = pthread_create(&tid_producer[i], NULL, producer, (void *)&thread_no[i])) != 0) {
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r)); exit(1);
        }
    }
    //Espera a que todos los hilos terminen sus funciones
    for (i = 0; i < 10; i++) {
        pthread_join(tid_producer[i], NULL);
    }
    //espera hasta que no queden mas mensajes pendientes, elimina el hilo encargado de los mensajes
    while (lines_to_print) sleep(1);
    pthread_cancel(tid_spooler);

    return 0;
}
//Funcion que ejecutan los hilos productores
void *producer(void *arg) {
    int my_id = *((int *)arg); //Obtiene el ID del hilo
    int count = 0; //Inicializa el contador
    //El productor va a tener 10 hilos
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&buf_mutex); //Bloquea al mutex para tener todo el acceso
        while (!buffers_available) //Espera hasta que haya un buffer para escribir
            pthread_cond_wait(&buf_cond, &buf_mutex);

        int j = buffer_index; //Obtiene en que buffer esta
        buffer_index = (buffer_index + 1) % MAX_BUFFERS; //Avanza al siguiente
        buffers_available--; //Reduce la cantidad de espacios libres
        sprintf(buf[j], "Thread %d: %d\n", my_id, ++count); //Escribe el mensaje
        lines_to_print++; //Aumenta el contador de pendientes
        pthread_cond_signal(&spool_cond); //Indica que hay nuevas lineas para mostrar al hilo encargado
        pthread_mutex_unlock(&buf_mutex); //Libera el mutex para que sigan otros

        sleep(1); //Espera
    }
    return NULL;
}
//Funcion que ejecuta el hilo spooler (consumidor)
void *spooler(void *arg) {
    while (1) { //Hasta que lo detengan
        pthread_mutex_lock(&buf_mutex); //Bloquea al mutex para tener todo el acceso
        while (!lines_to_print) //Cuando hay lineas pendientes de imprimir
            pthread_cond_wait(&spool_cond, &buf_mutex);

        printf("%s", buf[buffer_print_index]); //Muestra la linea de donde esta ahora mismo
        lines_to_print--; //Reduce la cantidad de lineas pendientes
        buffer_print_index = (buffer_print_index + 1) % MAX_BUFFERS; //Avanza al siguiente
        buffers_available++; //Libera espacio en el buffer
        pthread_cond_signal(&buf_cond); //Le indica a los productores que hay mas espacio
        pthread_mutex_unlock(&buf_mutex); //Libera el mutex para que sigan otros
    }
    return NULL;
}

/******************************************************************************
 * RESUMEN:
 * Este programa corresponde a la segunda Activiad del taller y demuestra la 
 * sincronización entre múltiples hilos dentro de un mismo proceso utilizando
 * la biblioteca pthread de POSIX.
 *
 * Componentes principales:
 *  - 10 hilos productores que generan líneas de texto.
 *  - 1 hilo spooler que actúa como consumidor e imprime los datos.
 *  - Un buffer circular protegido con un mutex.
 *  - Dos variables de condición para coordinar el acceso concurrente.
 *
 * Lógica de sincronización:
 *  - Los productores esperan hasta que haya espacio disponible en el buffer.
 *  - El spooler espera hasta que haya líneas listas para imprimir.
 *  - El mutex evita el acceso simultáneo a la sección crítica.
 * 
 * Resultado:
 *  Este ejercicio refleja un sistema concurrente bien coordinado,
 *  donde se evidencia el correcto uso de exclusión mutua y comunicación
 *  entre hilos mediante variables de condición.
 *
 *
 * IMPORTANCIA:
 * En esta actividad todo fue también de suma importancia. Sin embargo, la 
 * sincronización lograda con las primitivas pthread_mutex y pthread_cond_t 
 * fue el elemento más destacado, ya que permitió aplicar directamente los 
 * principios de concurrencia vistos en clase dentro de un mismo proceso.
 * 
 * El uso combinado del mutex y las variables de condición garantizó un 
 * control eficiente del acceso al recurso compartido, manteniendo el flujo 
 * productor–consumidor con total exclusión mutua.
 *****************************************************************************/
