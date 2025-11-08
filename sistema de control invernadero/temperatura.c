#include "temperatura.h"
#include "eventos.h"
#include "zonas.h"
#include <unistd.h>

float leer_temperatura_simulada(void) {
    
    float temperatura = 15.0 + ((float)rand() / RAND_MAX) * 20.0;
    return temperatura;
}

void ver_temperatura_actual(void) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    int i;
    
    if (!cargar_zonas(&zonas, &num_zonas) || num_zonas == 0) {
        printf("Error: No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== TEMPERATURA ACTUAL ===\n");
    printf("%-10s %-30s %-15s %-15s %-20s\n", 
           "ID", "Nombre", "Temperatura", "Umbral", "Ventilador");
    printf("--------------------------------------------------------------------------------\n");
    
    for (i = 0; i < num_zonas; i++) {
        float temperatura = leer_temperatura_simulada();
        int estado_anterior = zonas[i].estado_ventilador;
        
        
        int nuevo_estado = controlar_ventilador_automatico(zonas[i].id_zona, temperatura);
        zonas[i].estado_ventilador = nuevo_estado;
        
        
        if (estado_anterior != nuevo_estado) {
            registrar_evento(zonas[i].id_zona, temperatura, nuevo_estado);
        }
        
        printf("%-10d %-30s %-15.2f %-15.2f %-20s\n",
               zonas[i].id_zona,
               zonas[i].nombre,
               temperatura,
               zonas[i].umbral,
               nuevo_estado ? "Encendido (Auto)" : "Apagado (Auto)");
    }
    
    
    guardar_zonas(zonas, num_zonas);
    free(zonas);
    printf("\n");
}

int controlar_ventilador_automatico(int id_zona, float temperatura) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    Zona* zona = NULL;
    int nuevo_estado;
    
    if (!cargar_zonas(&zonas, &num_zonas)) {
        return 0;
    }
    
    zona = buscar_zona_por_id(zonas, num_zonas, id_zona);
    if (zona == NULL) {
        free(zonas);
        return 0;
    }
    
    
    nuevo_estado = (temperatura > zona->umbral) ? 1 : 0;
    
    free(zonas);
    return nuevo_estado;
}

void activar_ventilador_manual(void) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    char nombre_zona[MAX_NOMBRE];
    int opcion;
    int indice;
    float temperatura_actual;
    
    if (!cargar_zonas(&zonas, &num_zonas) || num_zonas == 0) {
        printf("Error: No hay zonas registradas.\n");
        return;
    }
    
    mostrar_zonas(zonas, num_zonas);
    
    printf("\n=== ACTIVAR VENTILADOR MANUALMENTE ===\n");
    printf("Nombre de la zona: ");
    limpiar_buffer();
    if (fgets(nombre_zona, sizeof(nombre_zona), stdin) == NULL) {
        free(zonas);
        return;
    }
    nombre_zona[strcspn(nombre_zona, "\n")] = '\0';
    
    indice = buscar_zona_por_nombre(zonas, num_zonas, nombre_zona);
    if (indice < 0) {
        printf("Error: Zona no encontrada.\n");
        free(zonas);
        return;
    }
    
    printf("\nZona seleccionada: %s (ID: %d)\n", zonas[indice].nombre, zonas[indice].id_zona);
    printf("Estado actual del ventilador: %s\n", 
           zonas[indice].estado_ventilador ? "Encendido" : "Apagado");
    
    printf("\nElija una opcion:\n");
    printf("1. Encender ventilador\n");
    printf("2. Apagar ventilador\n");
    
    if (scanf("%d", &opcion) != 1 || (opcion != 1 && opcion != 2)) {
        printf("Error: Opcion invalida.\n");
        limpiar_buffer();
        free(zonas);
        return;
    }
    limpiar_buffer();
    
    int estado_anterior = zonas[indice].estado_ventilador;
    zonas[indice].estado_ventilador = (opcion == 1) ? 1 : 0;
    

    temperatura_actual = leer_temperatura_simulada();
    
    
    registrar_evento(zonas[indice].id_zona, temperatura_actual, zonas[indice].estado_ventilador);
    
    
    if (guardar_zonas(zonas, num_zonas)) {
        printf("\nVentilador %s exitosamente.\n", 
               zonas[indice].estado_ventilador ? "encendido" : "apagado");
        printf("Temperatura registrada: %.2f°C\n", temperatura_actual);
        if (estado_anterior != zonas[indice].estado_ventilador) {
            printf("Evento registrado en el historial.\n");
        }
    } else {
        printf("Error: No se pudo guardar el cambio.\n");
    }
    
    free(zonas);
}

void simular_monitoreo_tiempo_real(void) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    char nombre_zona[MAX_NOMBRE];
    int indice;
    int ciclos;
    int intervalo;
    int i;
    
    if (!cargar_zonas(&zonas, &num_zonas) || num_zonas == 0) {
        printf("Error: No hay zonas registradas.\n");
        return;
    }
    
    mostrar_zonas(zonas, num_zonas);
    
    printf("\n=== SIMULACION DE MONITOREO EN TIEMPO REAL ===\n");
    printf("Nombre de la zona a monitorear: ");
    limpiar_buffer();
    if (fgets(nombre_zona, sizeof(nombre_zona), stdin) == NULL) {
        free(zonas);
        return;
    }
    nombre_zona[strcspn(nombre_zona, "\n")] = '\0';
    
    indice = buscar_zona_por_nombre(zonas, num_zonas, nombre_zona);
    if (indice < 0) {
        printf("Error: Zona no encontrada.\n");
        free(zonas);
        return;
    }
    
    printf("Numero de ciclos de lectura: ");
    if (scanf("%d", &ciclos) != 1 || ciclos <= 0) {
        printf("Error: Numero de ciclos invalido.\n");
        limpiar_buffer();
        free(zonas);
        return;
    }
    
    printf("Intervalo entre lecturas (segundos): ");
    if (scanf("%d", &intervalo) != 1 || intervalo <= 0) {
        printf("Error: Intervalo invalido.\n");
        limpiar_buffer();
        free(zonas);
        return;
    }
    limpiar_buffer();
    
    printf("\n=== Monitoreo en tiempo real: %s ===\n", zonas[indice].nombre);
    printf("Umbral: %.2f°C\n\n", zonas[indice].umbral);
    printf("%-15s %-15s %-20s\n", "Temperatura", "Umbral", "Ventilador");
    printf("------------------------------------------------------------\n");
    
    for (i = 0; i < ciclos; i++) {
        float temperatura = leer_temperatura_simulada();
        int estado_anterior = zonas[indice].estado_ventilador;
        int nuevo_estado = controlar_ventilador_automatico(zonas[indice].id_zona, temperatura);
        
        zonas[indice].estado_ventilador = nuevo_estado;
        
        
        if (estado_anterior != nuevo_estado) {
            registrar_evento(zonas[indice].id_zona, temperatura, nuevo_estado);
        }
        
        printf("%-15.2f %-15.2f %-20s",
               temperatura,
               zonas[indice].umbral,
               nuevo_estado ? "Encendido (Auto)" : "Apagado (Auto)");
        
        if (estado_anterior != nuevo_estado) {
            printf(" [CAMBIO]");
        }
        printf("\n");
        
        
        guardar_zonas(zonas, num_zonas);
        
        if (i < ciclos - 1) {
            sleep(intervalo);
        }
    }
    
    free(zonas);
    printf("\nMonitoreo completado.\n");
}

