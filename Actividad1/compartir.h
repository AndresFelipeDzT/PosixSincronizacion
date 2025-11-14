/******************************************************
* Fecha: 11/11/2025
* Pontificia Universidad Javeriana
* Profesor: J. Corredor, PhD
* Autor(es): Alejandro Beltran, Mauricio Beltran & Andres Diaz
* Materia: Sistemas Operativos
* Tema: Taller 03 - compartir.h (Actividad 1)
******************************************************/
#ifndef COMPARTIR_H
#define COMPARTIR_H

#define BUFFER 5 //Tamano del buffer compartido
//Estructura de datos compartidos entre productor y consumidor
typedef struct {
    int bus[BUFFER]; //Arreglo en donde estan los elementos para transportar
    int entrada; //Indice para saber donde el productor se insertan los datos
    int salida; //Indice para saber donde el consumidor obtiene los datos
} compartir_datos; //Nombre de la estructura

#endif

/*************************************************************************
 * RESUMEN:
 * Este archivo define la estructura de datos compartida entre los procesos
 * productor y consumidor de la primera Actividad. La estructura "compartir_datos"
 * actúa como un buffer circular almacenado en memoria compartida shm_open,
 * permitiendo que ambos procesos intercambien información de manera ordenada.
 * 
 * Cada posición del arreglo o bus representa un espacio en el buffer donde
 * el productor deposita un dato y el consumidor lo extrae. Los índices
 * "entrada" y "salida" controlan el flujo del buffer y evitan condiciones de
 * carrera mediante la sincronización con semáforos POSIX con nombre.
 **************************************************************************/