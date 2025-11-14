/******************************************************
* Fecha: 11/11/2025
* Pontificia Universidad Javeriana
* Profesor: J. Corredor, PhD
* Autor(es): Alejandro Beltran, Mauricio Beltran & Andres Diaz
* Materia: Sistemas Operativos
* Tema: Taller 03 -  Productor Semaforos POSIX (Actividad 1)
******************************************************/

#include <stdio.h>      // Libreria para funciones de entrada y salida
#include <stdlib.h>     // Libreria para manejo de memoria y exit
#include <semaphore.h>  // Libreria para usar semaforos
#include <fcntl.h>      // Libreria para flags de apertura
#include <sys/mman.h>   // Libreria para memoria compartida
#include <unistd.h>     // Libreria para sleep y close
#include "compartir.h"  // Que incluya compartir.h

int main() {
    //Se crea un semaforo vacio que puede tener tantos datos como el buffer
    sem_t *vacio = sem_open("/vacio", O_CREAT, 0644, BUFFER);
    //Se crea un semaforo lleno inicializado en 0
    sem_t *lleno = sem_open("/lleno", O_CREAT, 0644, 0);
    //Si no logro crear alguno de los semaforos
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open"); //Muestra el mensaje de error
        exit(EXIT_FAILURE); //Lo termina
    }
    //Crea la memoria compartida
    int shm_fd = shm_open("/memoria_compartida", O_CREAT | O_RDWR, 0644);
    //Si no logro crearla
    if (shm_fd < 0) {
        perror("shm_open"); //Muestra el mensaje de error
        exit(EXIT_FAILURE); //Lo termina
    }
    //Define el tamano de la memoria compartida basado en compartir_datos
    ftruncate(shm_fd, sizeof(compartir_datos));
    //Se le asigna una zona compartida de memoria a traves de map
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    //Si no logro asignarla
    if (compartir == MAP_FAILED) {
        perror("mmap"); //Muestra el mensaje de error
        exit(1); //Lo termina
    }

    compartir->entrada = 0; //Indicador para escribir datos
    compartir->salida = 0; //Indicador para leer datos
    //Un bucle de 1 a 10 iteraciones
    for (int i = 1; i <= 10; i++) {
        sem_wait(vacio); //Espera hasta que este el semaforo vacio
        compartir->bus[compartir->entrada] = i; //Escribe en ese espacio del bus de entrada el valor
        printf("Productor: Produce %d\n", i); //Muestra el valor
        //Actualiza la posicion de entrada
        compartir->entrada = (compartir->entrada + 1) % BUFFER;
        sem_post(lleno); //Indica que hay un nuevo elemento para el consumidor
        sleep(1); //Espera por que ahora le toca al consumidor antes de generar el nuevo dato
    }
    //Elimina el map de lo de memoria compartida
    munmap(compartir, sizeof(compartir_datos));
    //Elimina el descriptor de memoria compartida
    close(shm_fd);
    //Cierra los dos semaforos
    sem_close(vacio);
    sem_close(lleno);
    //Elimina los dos semaforos
    sem_unlink("/vacio");
    sem_unlink("/lleno");
    //Elimina el espacio de memoria compartida
    shm_unlink("/memoria_compartida");

    return 0;
}

/******************************************************************************
 * RESUMEN:
 * Este programa implementa el proceso productor del modelo Productor–Consumidor.
 * Se conecta a la memoria compartida definida en "compartir.h" y utiliza
 * semáforos POSIX con nombre para garantizar que nunca se escriba en un
 * buffer lleno. 
 *
 * Flujo lógico:
 * 1. Espera un espacio disponible (sem_wait(vacio)).
 * 2. Escribe un valor en el buffer compartido.
 * 3. Incrementa el semáforo "lleno" (sem_post(lleno)).
 * 
 * De esta forma, la escritura y la lectura son coordinadas entre procesos,
 * evitando interferencias y garantizando la integridad de los datos.
 *
 *
 * IMPORTANCIA:
 * Esta actividad fue fundamental para comprender la comunicación y sincronización
 * entre procesos usando memoria compartida y semáforos POSIX con nombre.
 * 
 * El punto más relevante es la creación y manejo de
 * los semáforos "/vacio" y "/lleno", junto con la escritura ordenada de los datos
 * dentro del buffer compartido. Cada valor producido se almacena de manera
 * secuencial, asegurando que nunca se sobrescriban posiciones ocupadas y que
 * el consumidor solo acceda a datos válidos.
 ******************************************************************************/
