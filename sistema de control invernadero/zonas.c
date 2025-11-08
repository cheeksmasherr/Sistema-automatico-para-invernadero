#include "zonas.h"

int obtener_siguiente_id_zona(void) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    int siguiente_id = 1;
    
    if (cargar_zonas(&zonas, &num_zonas)) {
        if (num_zonas > 0) {
            int max_id = 0;
            int i;
            for (i = 0; i < num_zonas; i++) {
                if (zonas[i].id_zona > max_id) {
                    max_id = zonas[i].id_zona;
                }
            }
            siguiente_id = max_id + 1;
        }
        free(zonas);
    }
    
    return siguiente_id;
}

int validar_id_unico(int id) {
    Zona* zonas = NULL;
    int num_zonas = 0;
    int i;
    int es_unico = 1;
    
    if (cargar_zonas(&zonas, &num_zonas)) {
        for (i = 0; i < num_zonas; i++) {
            if (zonas[i].id_zona == id) {
                es_unico = 0;
                break;
            }
        }
        free(zonas);
    }
    
    return es_unico;
}

int cargar_zonas(Zona** zonas, int* num_zonas) {
    FILE* archivo = fopen(ARCHIVO_ZONAS, "rb");
    if (archivo == NULL) {
        *zonas = NULL;
        *num_zonas = 0;
        return 0;
    }
    
    // Obtener tamaño del archivo
    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    
    *num_zonas = tamano / sizeof(Zona);
    
    if (*num_zonas > 0) {
        *zonas = (Zona*)malloc(*num_zonas * sizeof(Zona));
        if (*zonas == NULL) {
            fclose(archivo);
            *num_zonas = 0;
            return 0;
        }
        
        fread(*zonas, sizeof(Zona), *num_zonas, archivo);
    } else {
        *zonas = NULL;
    }
    
    fclose(archivo);
    return 1;
}

int guardar_zonas(Zona* zonas, int num_zonas) {
    FILE* archivo = fopen(ARCHIVO_ZONAS, "wb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo de zonas.\n");
        return 0;
    }
    
    if (num_zonas > 0) {
        fwrite(zonas, sizeof(Zona), num_zonas, archivo);
    }
    
    fclose(archivo);
    return 1;
}

void mostrar_zonas(Zona* zonas, int num_zonas) {
    int i;
    if (num_zonas == 0) {
        printf("No hay zonas registradas.\n");
        return;
    }
    
    printf("\n=== ZONAS REGISTRADAS ===\n");
    printf("%-10s %-30s %-15s %-20s\n", "ID", "Nombre", "Umbral (°C)", "Ventilador");
    printf("------------------------------------------------------------------------\n");
    
    for (i = 0; i < num_zonas; i++) {
        printf("%-10d %-30s %-15.2f %-20s\n",
               zonas[i].id_zona,
               zonas[i].nombre,
               zonas[i].umbral,
               zonas[i].estado_ventilador ? "Encendido" : "Apagado");
    }
    printf("\n");
}

int registrar_zona(void) {
    Zona nueva_zona;
    Zona* zonas = NULL;
    int num_zonas = 0;
    char nombre[MAX_NOMBRE];
    float umbral;
    int id;
    
    printf("\n=== REGISTRO DE ZONA ===\n");
    
    // Solicitar ID
    printf("ID de zona (o 0 para auto-generar): ");
    if (scanf("%d", &id) != 1) {
        printf("Error: ID invalido.\n");
        limpiar_buffer();
        return 0;
    }
    limpiar_buffer();
    
    if (id == 0) {
        id = obtener_siguiente_id_zona();
        printf("ID auto-generado: %d\n", id);
    } else {
        if (!validar_id_unico(id)) {
            printf("Error: El ID %d ya existe. Por favor, use otro ID.\n", id);
            return 0;
        }
    }
    
    nueva_zona.id_zona = id;
    
    
    printf("Nombre de la zona: ");
    if (fgets(nombre, sizeof(nombre), stdin) == NULL) {
        printf("Error al leer el nombre.\n");
        return 0;
    }
    nombre[strcspn(nombre, "\n")] = '\0';
    
    if (!validar_string_no_vacio(nombre)) {
        printf("Error: El nombre no puede estar vacio.\n");
        return 0;
    }
    
    
    if (cargar_zonas(&zonas, &num_zonas)) {
        if (buscar_zona_por_nombre(zonas, num_zonas, nombre) >= 0) {
            printf("Error: Ya existe una zona con ese nombre.\n");
            free(zonas);
            return 0;
        }
    }
    
    strcpy(nueva_zona.nombre, nombre);
    
    
    printf("Umbral de temperatura (°C) [default: %.1f]: ", UMBRAL_DEFAULT);
    if (scanf("%f", &umbral) != 1) {
        umbral = UMBRAL_DEFAULT;
        printf("Usando umbral por defecto: %.1f°C\n", umbral);
    }
    limpiar_buffer();
    
    if (!validar_numero_positivo(umbral)) {
        printf("Error: El umbral debe ser un numero positivo.\n");
        if (zonas != NULL) free(zonas);
        return 0;
    }
    
    nueva_zona.umbral = umbral;
    nueva_zona.estado_ventilador = 0; 
    
    
    if (!cargar_zonas(&zonas, &num_zonas)) {
        zonas = NULL;
        num_zonas = 0;
    }
    
    
    Zona* zonas_actualizadas = (Zona*)realloc(zonas, (num_zonas + 1) * sizeof(Zona));
    if (zonas_actualizadas == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        if (zonas != NULL) free(zonas);
        return 0;
    }
    
    zonas = zonas_actualizadas;
    zonas[num_zonas] = nueva_zona;
    num_zonas++;
    
    
    if (guardar_zonas(zonas, num_zonas)) {
        printf("\nZona registrada exitosamente!\n");
        printf("ID: %d\n", nueva_zona.id_zona);
        printf("Nombre: %s\n", nueva_zona.nombre);
        printf("Umbral: %.2f°C\n", nueva_zona.umbral);
        free(zonas);
        return 1;
    } else {
        printf("Error: No se pudo guardar la zona.\n");
        free(zonas);
        return 0;
    }
}

