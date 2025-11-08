#include "reportes.h"
#include "eventos.h"
#include "zonas.h"
#include <math.h>

void buscar_eventos_por_filtro(void) {
    NodoEvento* lista = cargar_eventos();
    NodoEvento* actual;
    char nombre_zona[MAX_NOMBRE];
    int opcion;
    float temp_min = 0, temp_max = 0;
    char fecha_inicio[11], fecha_fin[11];
    int encontrados = 0;
    Zona* zonas = NULL;
    int num_zonas = 0;
    int id_zona_busqueda = -1;
    
    if (lista == NULL) {
        printf("No hay eventos registrados.\n");
        return;
    }
    
    cargar_zonas(&zonas, &num_zonas);
    
    printf("\n=== BUSCAR EVENTOS ===\n");
    printf("Nombre de la zona: ");
    limpiar_buffer();
    if (fgets(nombre_zona, sizeof(nombre_zona), stdin) == NULL) {
        liberar_lista_eventos(lista);
        if (zonas != NULL) free(zonas);
        return;
    }
    nombre_zona[strcspn(nombre_zona, "\n")] = '\0';
    
    int indice = buscar_zona_por_nombre(zonas, num_zonas, nombre_zona);
    if (indice < 0) {
        printf("Error: Zona no encontrada.\n");
        liberar_lista_eventos(lista);
        if (zonas != NULL) free(zonas);
        return;
    }
    
    id_zona_busqueda = zonas[indice].id_zona;
    printf("Zona seleccionada: %s (ID: %d)\n\n", zonas[indice].nombre, id_zona_busqueda);
    
    printf("Filtrar por:\n");
    printf("1. Rango de temperatura\n");
    printf("2. Rango de fechas\n");
    printf("Opcion: ");
    
    if (scanf("%d", &opcion) != 1 || (opcion != 1 && opcion != 2)) {
        printf("Error: Opcion invalida.\n");
        limpiar_buffer();
        liberar_lista_eventos(lista);
        if (zonas != NULL) free(zonas);
        return;
    }
    limpiar_buffer();
    
    if (opcion == 1) {
        printf("Temperatura minima: ");
        if (scanf("%f", &temp_min) != 1) {
            printf("Error: Temperatura invalida.\n");
            limpiar_buffer();
            liberar_lista_eventos(lista);
            if (zonas != NULL) free(zonas);
            return;
        }
        
        printf("Temperatura maxima: ");
        if (scanf("%f", &temp_max) != 1) {
            printf("Error: Temperatura invalida.\n");
            limpiar_buffer();
            liberar_lista_eventos(lista);
            if (zonas != NULL) free(zonas);
            return;
        }
        limpiar_buffer();
        
        if (temp_min > temp_max) {
            printf("Error: La temperatura minima no puede ser mayor que la maxima.\n");
            liberar_lista_eventos(lista);
            if (zonas != NULL) free(zonas);
            return;
        }
    } else {
        printf("Fecha de inicio (YYYY-MM-DD): ");
        if (fgets(fecha_inicio, sizeof(fecha_inicio), stdin) == NULL) {
            liberar_lista_eventos(lista);
            if (zonas != NULL) free(zonas);
            return;
        }
        fecha_inicio[strcspn(fecha_inicio, "\n")] = '\0';
        
        printf("Fecha de fin (YYYY-MM-DD): ");
        if (fgets(fecha_fin, sizeof(fecha_fin), stdin) == NULL) {
            liberar_lista_eventos(lista);
            if (zonas != NULL) free(zonas);
            return;
        }
        fecha_fin[strcspn(fecha_fin, "\n")] = '\0';
    }
    
    printf("\n=== EVENTOS ENCONTRADOS ===\n");
    printf("%-12s %-10s %-15s %-20s\n",
           "Fecha", "Hora", "Temperatura", "Ventilador");
    printf("------------------------------------------------------------\n");
    
    actual = lista;
    while (actual != NULL) {
        int coincide = 0;
        
        
        if (actual->evento.id_zona == id_zona_busqueda) {
            if (opcion == 1) {
                
                if (actual->evento.temperatura >= temp_min && 
                    actual->evento.temperatura <= temp_max) {
                    coincide = 1;
                }
            } else {
                
                if (comparar_fechas(actual->evento.fecha, fecha_inicio) >= 0 &&
                    comparar_fechas(actual->evento.fecha, fecha_fin) <= 0) {
                    coincide = 1;
                }
            }
            
            if (coincide) {
                printf("%-12s %-10s %-15.2f %-20s\n",
                       actual->evento.fecha,
                       actual->evento.hora,
                       actual->evento.temperatura,
                       actual->evento.estado_ventilador ? "Encendido" : "Apagado");
                encontrados++;
            }
        }
        
        actual = actual->siguiente;
    }
    
    printf("\nTotal de eventos encontrados: %d\n", encontrados);
    
    liberar_lista_eventos(lista);
    if (zonas != NULL) free(zonas);
}

void generar_reporte_estadistico(void) {
    NodoEvento* lista = cargar_eventos();
    NodoEvento* actual;
    Zona* zonas = NULL;
    int num_zonas = 0;
    int i;
    
    if (lista == NULL) {
        printf("No hay eventos registrados para generar estadisticas.\n");
        return;
    }
    
    cargar_zonas(&zonas, &num_zonas);
    
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        liberar_lista_eventos(lista);
        return;
    }
    
    printf("\n=== REPORTE ESTADISTICO ===\n\n");
    
    
    float temp_max_global = -999.0;
    float temp_min_global = 999.0;
    float suma_global = 0.0;
    int total_eventos_global = 0;
    char zona_max_temp[MAX_NOMBRE] = "";
    char zona_min_temp[MAX_NOMBRE] = "";
    char zona_promedio_max[MAX_NOMBRE] = "";
    float promedio_max = -999.0;
    
    for (i = 0; i < num_zonas; i++) {
        actual = lista;
        float temp_max = -999.0;
        float temp_min = 999.0;
        float suma = 0.0;
        int contador = 0;
        
        while (actual != NULL) {
            if (actual->evento.id_zona == zonas[i].id_zona) {
                if (actual->evento.temperatura > temp_max) {
                    temp_max = actual->evento.temperatura;
                }
                if (actual->evento.temperatura < temp_min) {
                    temp_min = actual->evento.temperatura;
                }
                suma += actual->evento.temperatura;
                contador++;
            }
            actual = actual->siguiente;
        }
        
        if (contador > 0) {
            float promedio = suma / contador;
            
            printf("Zona: %s (ID: %d)\n", zonas[i].nombre, zonas[i].id_zona);
            printf("  Temperatura maxima: %.2f°C\n", temp_max);
            printf("  Temperatura minima: %.2f°C\n", temp_min);
            printf("  Temperatura promedio: %.2f°C\n", promedio);
            printf("  Total de eventos: %d\n\n", contador);
            
            
            if (temp_max > temp_max_global) {
                temp_max_global = temp_max;
                strcpy(zona_max_temp, zonas[i].nombre);
            }
            if (temp_min < temp_min_global) {
                temp_min_global = temp_min;
                strcpy(zona_min_temp, zonas[i].nombre);
            }
            if (promedio > promedio_max) {
                promedio_max = promedio;
                strcpy(zona_promedio_max, zonas[i].nombre);
            }
            
            suma_global += suma;
            total_eventos_global += contador;
        } else {
            printf("Zona: %s (ID: %d)\n", zonas[i].nombre, zonas[i].id_zona);
            printf("  No hay eventos registrados para esta zona.\n\n");
        }
    }
    
    
    float promedio_global = (total_eventos_global > 0) ? suma_global / total_eventos_global : 0.0;
    
    printf("=== ESTADISTICAS GLOBALES ===\n");
    if (total_eventos_global > 0) {
        printf("Temperatura maxima global: %.2f°C (Zona: %s)\n", temp_max_global, zona_max_temp);
        printf("Temperatura minima global: %.2f°C (Zona: %s)\n", temp_min_global, zona_min_temp);
        printf("Temperatura promedio global: %.2f°C\n", promedio_global);
        printf("Zona con mayor promedio: %s (%.2f°C)\n", zona_promedio_max, promedio_max);
    } else {
        printf("No hay eventos para calcular estadisticas globales.\n");
    }
    
    liberar_lista_eventos(lista);
    if (zonas != NULL) free(zonas);
}

void exportar_historial_csv(void) {
    NodoEvento* lista = cargar_eventos();
    NodoEvento* actual;
    FILE* archivo_csv;
    Zona* zonas = NULL;
    int num_zonas = 0;
    char nombre_archivo[200];
    int total_registros = 0;
    
    if (lista == NULL) {
        printf("No hay eventos para exportar.\n");
        return;
    }
    
    cargar_zonas(&zonas, &num_zonas);
    
    printf("\n=== EXPORTAR HISTORIAL A CSV ===\n");
    printf("Nombre del archivo (sin extension): ");
    limpiar_buffer();
    if (fgets(nombre_archivo, sizeof(nombre_archivo), stdin) == NULL) {
        liberar_lista_eventos(lista);
        if (zonas != NULL) free(zonas);
        return;
    }
    nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0';
    
    
    char archivo_completo[250];
    if (strstr(nombre_archivo, ".csv") == NULL) {
        sprintf(archivo_completo, "%s.csv", nombre_archivo);
    } else {
        strcpy(archivo_completo, nombre_archivo);
    }
    
    archivo_csv = fopen(archivo_completo, "w");
    if (archivo_csv == NULL) {
        printf("Error: No se pudo crear el archivo CSV.\n");
        liberar_lista_eventos(lista);
        if (zonas != NULL) free(zonas);
        return;
    }
    
    
    fprintf(archivo_csv, "ID Zona,Nombre Zona,Fecha,Hora,Temperatura,Estado Ventilador\n");
    
    
    actual = lista;
    while (actual != NULL) {
        char nombre_zona[MAX_NOMBRE] = "Desconocida";
        Zona* zona = buscar_zona_por_id(zonas, num_zonas, actual->evento.id_zona);
        if (zona != NULL) {
            strcpy(nombre_zona, zona->nombre);
        }
        
        fprintf(archivo_csv, "%d,%s,%s,%s,%.2f,%s\n",
                actual->evento.id_zona,
                nombre_zona,
                actual->evento.fecha,
                actual->evento.hora,
                actual->evento.temperatura,
                actual->evento.estado_ventilador ? "Encendido" : "Apagado");
        
        actual = actual->siguiente;
        total_registros++;
    }
    
    fclose(archivo_csv);
    
    printf("\nHistorial exportado exitosamente a: %s\n", archivo_completo);
    printf("Total de registros: %d\n", total_registros);
    
    liberar_lista_eventos(lista);
    if (zonas != NULL) free(zonas);
}

