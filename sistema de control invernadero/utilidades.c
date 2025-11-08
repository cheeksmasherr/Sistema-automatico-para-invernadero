#include "utilidades.h"

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validar_numero_positivo(float num) {
    return num > 0;
}

int validar_string_no_vacio(const char* str) {
    if (str == NULL) return 0;
    int i = 0;
    while (str[i] != '\0') {
        if (!isspace(str[i])) {
            return 1;
        }
        i++;
    }
    return 0;
}

void obtener_fecha_actual(char* fecha) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    sprintf(fecha, "%04d-%02d-%02d", 
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1,
            tm_info->tm_mday);
}

void obtener_hora_actual(char* hora) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    sprintf(hora, "%02d:%02d:%02d",
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec);
}

int comparar_fechas(const char* fecha1, const char* fecha2) {
    return strcmp(fecha1, fecha2);
}

void convertir_minusculas(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }
}

int buscar_zona_por_nombre(Zona* zonas, int num_zonas, const char* nombre) {
    int i;
    char nombre_busqueda[MAX_NOMBRE];
    char nombre_zona[MAX_NOMBRE];
    
    strcpy(nombre_busqueda, nombre);
    convertir_minusculas(nombre_busqueda);
    
    for (i = 0; i < num_zonas; i++) {
        strcpy(nombre_zona, zonas[i].nombre);
        convertir_minusculas(nombre_zona);
        if (strcmp(nombre_busqueda, nombre_zona) == 0) {
            return i;
        }
    }
    return -1;
}

Zona* buscar_zona_por_id(Zona* zonas, int num_zonas, int id) {
    int i;
    for (i = 0; i < num_zonas; i++) {
        if (zonas[i].id_zona == id) {
            return &zonas[i];
        }
    }
    return NULL;
}

void pausa(void) {
    printf("\nPresione Enter para continuar...");
    limpiar_buffer();
    getchar();
}

