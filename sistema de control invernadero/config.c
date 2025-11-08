#include "config.h"
#include "eventos.h"
#include "zonas.h"

void configurar_umbral_zona(void) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    char nombre_zona[MAX_NOMBRE];
    float nuevo_umbral;
    int indice;
    
    if (!cargar_zonas(&zonas, &num_zonas) || num_zonas == 0) {
        printf("Error: No hay zonas registradas.\n");
        return;
    }
    
    mostrar_zonas(zonas, num_zonas);
    
    printf("\n=== CONFIGURAR UMBRAL DE TEMPERATURA ===\n");
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
    printf("Umbral actual: %.2f°C\n", zonas[indice].umbral);
    
    printf("Nuevo umbral de temperatura (°C): ");
    if (scanf("%f", &nuevo_umbral) != 1) {
        printf("Error: Umbral invalido.\n");
        limpiar_buffer();
        free(zonas);
        return;
    }
    limpiar_buffer();
    
    if (!validar_numero_positivo(nuevo_umbral)) {
        printf("Error: El umbral debe ser un numero positivo.\n");
        free(zonas);
        return;
    }
    
    zonas[indice].umbral = nuevo_umbral;
    
    if (guardar_zonas(zonas, num_zonas)) {
        printf("\nUmbral actualizado exitosamente a %.2f°C para la zona %s.\n",
               nuevo_umbral, zonas[indice].nombre);
    } else {
        printf("Error: No se pudo guardar la configuracion.\n");
    }
    
    free(zonas);
}

void restaurar_configuracion_default(void) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    char nombre_zona[MAX_NOMBRE];
    int indice;
    char confirmacion;
    int limpiar_historial;
    
    if (!cargar_zonas(&zonas, &num_zonas) || num_zonas == 0) {
        printf("Error: No hay zonas registradas.\n");
        return;
    }
    
    mostrar_zonas(zonas, num_zonas);
    
    printf("\n=== RESTAURAR CONFIGURACION POR DEFECTO ===\n");
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
    printf("Umbral actual: %.2f°C\n", zonas[indice].umbral);
    printf("Umbral por defecto: %.2f°C\n", UMBRAL_DEFAULT);
    
    printf("\n¿Desea limpiar el historial de eventos para esta zona? (s/n): ");
    if (scanf(" %c", &confirmacion) != 1) {
        limpiar_buffer();
        free(zonas);
        return;
    }
    limpiar_buffer();
    
    limpiar_historial = (confirmacion == 's' || confirmacion == 'S') ? 1 : 0;
    
    // Restaurar umbral
    zonas[indice].umbral = UMBRAL_DEFAULT;
    zonas[indice].estado_ventilador = 0;
    
    if (guardar_zonas(zonas, num_zonas)) {
        printf("\nConfiguracion restaurada exitosamente.\n");
        printf("Umbral restaurado a %.2f°C\n", UMBRAL_DEFAULT);
        
        if (limpiar_historial) {
            // Limpiar eventos de esta zona
            NodoEvento* lista = cargar_eventos();
            NodoEvento* actual;
            FILE* archivo_nuevo = fopen(ARCHIVO_EVENTOS, "wb");
            
            if (archivo_nuevo != NULL && lista != NULL) {
                actual = lista;
                while (actual != NULL) {
                    
                    if (actual->evento.id_zona != zonas[indice].id_zona) {
                        fwrite(&actual->evento, sizeof(Evento), 1, archivo_nuevo);
                    }
                    actual = actual->siguiente;
                }
                fclose(archivo_nuevo);
                printf("Historial de eventos de la zona %s limpiado.\n", zonas[indice].nombre);
            }
            
            if (lista != NULL) {
                liberar_lista_eventos(lista);
            }
        }
    } else {
        printf("Error: No se pudo guardar la configuracion.\n");
    }
    
    free(zonas);
}

