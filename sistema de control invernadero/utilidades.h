#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Constantes
#define MAX_NOMBRE 100
#define MAX_PASSWORD 50
#define MAX_ZONAS 50
#define ARCHIVO_USUARIOS "usuarios.dat"
#define ARCHIVO_ZONAS "zonas.dat"
#define ARCHIVO_EVENTOS "eventos.dat"
#define UMBRAL_DEFAULT 25.0

// Estructuras
typedef struct {
    int id_zona;
    char nombre[MAX_NOMBRE];
    float umbral;
    int estado_ventilador; // 0 = apagado, 1 = encendido
} Zona;

typedef struct {
    int id_zona;
    char fecha[11]; // formato: YYYY-MM-DD
    char hora[9];   // formato: HH:MM:SS
    float temperatura;
    int estado_ventilador;
} Evento;

typedef struct {
    char nombre_usuario[MAX_NOMBRE];
    char password[MAX_PASSWORD];
} Usuario;

typedef struct NodoEvento {
    Evento evento;
    struct NodoEvento* siguiente;
} NodoEvento;

// Prototipos de funciones de utilidades
void limpiar_buffer(void);
int validar_numero_positivo(float num);
int validar_string_no_vacio(const char* str);
void obtener_fecha_actual(char* fecha);
void obtener_hora_actual(char* hora);
int comparar_fechas(const char* fecha1, const char* fecha2);
void convertir_minusculas(char* str);
int buscar_zona_por_nombre(Zona* zonas, int num_zonas, const char* nombre);
Zona* buscar_zona_por_id(Zona* zonas, int num_zonas, int id);
void pausa(void);

#endif

