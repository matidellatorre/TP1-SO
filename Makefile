BINARY=bin 
CODEDIRS=.
INCDIRS=. ./include/

CC=gcc
DEPFLAGS=-MP -MD #Flags de dependecias, le dicen a GCC que cuando consturya el programa, no solo genere el programa sino que tambien genere las dependencias para que make las entienda.
CFLAGS= -Wall #-g $(foreach D, $(INCDIRS), -I$(D)) $(DEPFLAGS) #Genero dichas dependencias con -I de forma iterativa
CFILES = $(foreach D, $(CODEDIRS), $(wildcard $(D)/*.c)) #Busco en los directorios que le dije los archivos .c
OBJECTS=$(patsubst %.c,%.o,$(CFILES)) #Este pattern substitution hace que por cada elemento en la lista de CFILES, que tome solo los que matchean .c (van a ser todos) y los convierta en .o. O sea que busqué los .o donde estaban sus respectivos .o
DEPFILES=$(patsubst %.c,%.d,$(CFILES)) #Este hace lo mismo pero con archivos .d, que son los que gcc genera para marcarle a make las dependencias gracias a los flags -MP -MD

all: $(BINARY)

$(BINARY): $(OBJECTS) #Para el binario necesito los archivos objeto
	$(CC) -o $@ $^ #Creo el binario (linkedito)

%.o:%.c #Para los archivos objeto necesito los .c
	$(CC) $(CFLAGS) -c -o $@ $^ #Creo los archivos objeto (compilo)

clean:
	rm -rf $(BINARY) *.o *.d #Borro el binario, los archivos objeto y los archivos de dependencias