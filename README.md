# Trabajo práctico N1 - Inter Process Communication

Grupo 3
ITBA

## Intrucciones de compilación

La compilación y linkedición del siguiente proyecto se realiza mediante Makefile. 

Basta con ejecutar el comando `make` en la carpeta raíz del proyecto para que éste compile y linkedite automáticamente.
Se crearán tres binarios: `solve`(aplicación), `slave`(esclavo) y `view`(vista).

## Intrucciones de ejecución

Para correr el programa, ejecutar cualquiera de los siguientes comandos en la carpeta raíz del proyecto:

`/.solve file1 file2 … file_n | ./view`

Primero `/.solve file1 file2 … file_n` que imprimirá shm_name por salida estándar, y luego, en otra terminal o en foreground, `/.solve shm_name`.

## Limpieza de binarios

Para limpiar tanto los binarios como los archivos objeto .o, ejecutar `make clean`.




