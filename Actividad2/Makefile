# ******************************************************
# Fecha: 11/11/2025
# Pontificia Universidad Javeriana
# Profesor: J. Corredor, PhD
# Autor(es): Alejandro Beltran, Mauricio Beltran & Andres Diaz
# Materia: Sistemas Operativos
# Tema: Taller 03 - Makefile (Actividad 2)
# ******************************************************
#Indica que hay que considerar todo aunque solo esta posixSincro
all: posixSincro
#Compila posixSincro.c y crea su ejecutable considera que es para hilos
posixSincro: posixSincro.c
	gcc -o posixSincro posixSincro.c -pthread
#Elimina los ejecutables
clean:
	rm -f posixSincro
