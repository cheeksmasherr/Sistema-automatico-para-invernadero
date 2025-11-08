#include "eventos.h"
#include "zonas.h"
#include <stdlib.h>

void registrar_evento(int id_zona, float temperatura, int estado_ventilador) {
    Evento nuevo_evento;
    
    nuevo_evento.id_zona = id_zona;
    nuevo_evento.temperatura = temperatura;
    nuevo_evento.estado_ventilador = estado_ventilador;
    
    obtener_fecha_actual(nuevo_evento.fecha);
    obtener_hora_actual(nuevo_evento.hora);
    
    
    guardar_evento_en_archivo(&nuevo_evento);
}

int guardar_evento_en_archivo(Evento* evento) {
    FILE* archivo = fopen(ARCHIVO_EVENTOS, "ab");
    if (archivo == NULL) {
        return 0;
    }
    
    fwrite(evento, sizeof(Evento), 1, archivo);
    fclose(archivo);
    return 1;
}

NodoEvento* cargar_eventos(void) {
    FILE* archivo = fopen(ARCHIVO_EVENTOS, "rb");
    if (archivo == NULL) {
        return NULL;
    }
    
    NodoEvento* lista = NULL;
    NodoEvento* ultimo = NULL;
    Evento evento;
    
    while (fread(&evento, sizeof(Evento), 1, archivo) == 1) {
        NodoEvento* nuevo_nodo = (NodoEvento*)malloc(sizeof(NodoEvento));
        if (nuevo_nodo == NULL) {
            liberar_lista_eventos(lista);
            fclose(archivo);
            return NULL;
        }
        
        nuevo_nodo->evento = evento;
        nuevo_nodo->siguiente = NULL;
        
        if (lista == NULL) {
            lista = nuevo_nodo;
            ultimo = nuevo_nodo;
        } else {
            ultimo->siguiente = nuevo_nodo;
            ultimo = nuevo_nodo;
        }
    }
    
    fclose(archivo);
    return lista;
}

void liberar_lista_eventos(NodoEvento* lista) {
    NodoEvento* actual = lista;
    NodoEvento* siguiente;
    
    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

void mostrar_historial_eventos(void) {
    NodoEvento* lista = cargar_eventos();
    NodoEvento* actual;
    Zona* zonas = NULL;
    int num_zonas = 0;
    int total_eventos = 0;
    
    if (lista == NULL) {
        printf("No hay eventos registrados.\n");
        return;
    }
    
    // Cargar zonas para mostrar nombres
    cargar_zonas(&zonas, &num_zonas);
    
    printf("\n=== HISTORIAL DE EVENTOS ===\n");
    printf("%-10s %-30s %-12s %-10s %-15s %-20s\n",
           "ID Zona", "Nombre Zona", "Fecha", "Hora", "Temperatura", "Ventilador");
    printf("-------------------------------------------------------------------------------------------\n");
    
    actual = lista;
    while (actual != NULL) {
        char nombre_zona[MAX_NOMBRE] = "Desconocida";
        Zona* zona = buscar_zona_por_id(zonas, num_zonas, actual->evento.id_zona);
        if (zona != NULL) {
            strcpy(nombre_zona, zona->nombre);
        }
        
        printf("%-10d %-30s %-12s %-10s %-15.2f %-20s\n",
               actual->evento.id_zona,
               nombre_zona,
               actual->evento.fecha,
               actual->evento.hora,
               actual->evento.temperatura,
               actual->evento.estado_ventilador ? "Encendido" : "Apagado");
        
        actual = actual->siguiente;
        total_eventos++;
    }
    
    printf("\nTotal de eventos: %d\n", total_eventos);
    
    if (zonas != NULL) {
        free(zonas);
    }
    liberar_lista_eventos(lista);
}

