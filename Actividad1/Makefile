# ******************************************************
# Fecha: 11/11/2025
# Pontificia Universidad Javeriana
# Profesor: J. Corredor, PhD
# Autor(es): Alejandro Beltran, Mauricio Beltran & Andres Diaz
# Materia: Sistemas Operativos
# Tema: Taller 03 - Makefile (Actividad 1)
# ******************************************************
# Utiliza gcc para compilar
CC = gcc
#Para adveretencias y uso de hilos y posix
CFLAGS = -Wall -pthread
#Indica que hay que considerar tanto al productor como al consumidor
all: productor consumidor
#Compila producer.c y crea su ejecutable
productor: producer.c compartir.h
	$(CC) $(CFLAGS) producer.c -o productor
#Compila consumer.c y crea su ejecutable
consumidor: consumer.c compartir.h
	$(CC) $(CFLAGS) consumer.c -o consumidor
#Elimina los ejecutables
clean:
	rm -f productor consumidor
