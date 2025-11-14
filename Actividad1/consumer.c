/******************************************************
* Fecha: 11/11/2025
* Pontificia Universidad Javeriana
* Profesor: J. Corredor, PhD
* Autor(es): Alejandro Beltran, Mauricio Beltran & Andres Diaz
* Materia: Sistemas Operativos
* Tema: Taller 03 - Consumidor Semaforos POSIX (Actividad 1)
******************************************************/

#include <stdio.h>      // Libreria para funciones de entrada y salida
#include <stdlib.h>     // Libreria para manejo de memoria y exit
#include <semaphore.h>  // Libreria para usar semaforos
#include <fcntl.h>      // Libreria para flags de apertura
#include <sys/mman.h>   // Libreria para memoria compartida
#include <unistd.h>     // Libreria para sleep y close
#include "compartir.h"  // Que incluya compartir.h

int main() {
    //Abre el semaforo vacio
    sem_t *vacio = sem_open("/vacio", 0);
    //Abre el semaforo que es para cuando esta lleno de datos
    sem_t *lleno = sem_open("/lleno", 0);
    //Si no logra abrirlos
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("sem_open"); //Muestra mensaje de error
        exit(EXIT_FAILURE); //Lo termina
    }
    //Abre la memeoria compartida que habilito desde productor
    int fd_compartido = shm_open("/memoria_compartida", O_RDWR, 0644);
    //Si no logro abrirla
    if (fd_compartido < 0) {
        perror("shm_open"); //Muestra mensaje de error
        exit(EXIT_FAILURE); //Lo termina
    }
    //Mapea la memoria compartida
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, fd_compartido, 0);
    //Si no logro mapearla
    if (compartir == MAP_FAILED) {
        perror("mmap"); //Muestra mensaje de error
        exit(1); //Lo termina
    }
    //Consume los 10 elementos que obtuvo del productor
    for (int i = 1; i <= 10; i++) {
        sem_wait(lleno); //Espera que haya elmentos disponibles
        int item = compartir->bus[compartir->salida]; //Los lee
        printf("Consumidor: Consume %d\n", item); //Indica que consumio
        compartir->salida = (compartir->salida + 1) % BUFFER; //Actualiza la posicion
        sem_post(vacio); //Incrementa los espacios vacios
        sleep(2); //Espera para que el productor tenga tiempo de enviar mas
    }

    munmap(compartir, sizeof(compartir_datos)); //Desmapea la memoria compartida
    close(fd_compartido); //Cierra el descriptor
    sem_close(vacio); //Cierra el semaforor vacio
    sem_close(lleno); //Cierra el semaforo lleno

    return 0;
}

/*********************************************************************************
 * RESUMEN:
 * Este programa implementa el proceso consumidor del modelo Productor–Consumidor.
 * Se conecta al mismo espacio de memoria compartida que el productor y utiliza
 * los semáforos POSIX con nombre "lleno" y "vacio" para controlar el acceso.
 *
 * Flujo lógico:
 * 1. Espera hasta que haya al menos un elemento disponible (sem_wait(lleno)).
 * 2. Lee y muestra el valor desde el buffer compartido.
 * 3. Libera el espacio incrementando el semáforo "vacio" (sem_post(vacio)).
 * 
 * Así se asegura una ejecución alternada y sincronizada entre los dos procesos,
 * demostrando el correcto uso de la memoria compartida y la comunicación entre procesos.
 *
 *
 * IMPORTANCIA:
 * Esta parte del taller permitió evidenciar la correcta lectura y consumo de datos
 * desde un espacio de memoria compartida, en coordinación con el proceso productor.
 * 
 * El código del consumidor se destaca por el uso de los semáforos POSIX "/vacio"
 * y "/lleno", los cuales regulan el acceso al buffer y garantizan la alternancia
 * entre los procesos. Gracias a este control, el consumidor espera de forma
 * sincronizada hasta que haya elementos disponibles, evitando condiciones de
 * carrera y lecturas inválidas.
 *********************************************************************************/
