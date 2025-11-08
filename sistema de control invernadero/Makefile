# Makefile para Sistema de Control de Temperatura - Invernadero
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = sistema_invernadero
SOURCES = main.c utilidades.c auth.c zonas.c temperatura.c eventos.c reportes.c config.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = utilidades.h auth.h zonas.h temperatura.h eventos.h reportes.h config.h

# Regla por defecto
all: $(TARGET)

# Compilar el programa principal
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) -lm

# Regla para compilar archivos objeto
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(TARGET) $(OBJECTS) *.o

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar archivos de datos
clean-data:
	rm -f usuarios.dat zonas.dat eventos.dat *.csv

# Limpiar todo (binarios y datos)
clean-all: clean clean-data

.PHONY: all clean run clean-data clean-all

