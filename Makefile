BINARY=solve
SLAVE_BINARY=slave 
VIEW_BINARY=view
CODEDIRS=.
INCDIRS=. ./include/

CC=gcc
CFLAGS= -Wall -g -std=c99 -D_GNU_SOURCE -pthread -lrt
CFILES = solve.c masterADT.c shmADT.c
OBJECTS=$(patsubst %.c,%.o,$(CFILES)) #Este pattern substitution hace que por cada elemento en la lista de CFILES, que tome solo los que matchean .c (van a ser todos) y los convierta en .o. O sea que busqué los .o donde estaban sus respectivos .o

all: $(BINARY) $(SLAVE_BINARY) $(VIEW_BINARY)

$(BINARY): $(CFILES) #Para el binario necesito los archivos objeto. Abajo: Creo el binario (linkedito)
	$(CC) $(CFLAGS) $^ -o $@ 

$(SLAVE_BINARY): slave.c #FALTA AGREGAR EL FLAG STD-C99
	$(CC) $(CFLAGS) $^ -o $@ 

$(VIEW_BINARY): shmADT.o
	$(CC) $(CFLAGS) -c view.c 
	$(CC) view.o $^ -lrt -o $@ 

clean: #Borro el binario, los archivos objeto y los archivos de dependencias
	rm -rf $(BINARY) $(SLAVE_BINARY)*.o *.d 