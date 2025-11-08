#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utilidades.h"
#include "auth.h"
#include "zonas.h"
#include "temperatura.h"
#include "eventos.h"
#include "reportes.h"
#include "config.h"

void mostrar_menu_principal(void);
void menu_zonas(void);
void menu_control_temperaturas(void);
void menu_consultas(void);
void menu_configuracion(void);

int main(void) {
    
    srand((unsigned int)time(NULL));
    
    
    inicializar_archivo_usuarios();
    
    
    if (!login()) {
        printf("Acceso denegado. Saliendo del sistema.\n");
        return 1;
    }
    
    
    int opcion;
    int salir = 0;
    
    while (!salir) {
        mostrar_menu_principal();
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            printf("Error: Opcion invalida.\n");
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();
        
        switch (opcion) {
            case 1:
                menu_zonas();
                break;
            case 2:
                menu_control_temperaturas();
                break;
            case 3:
                menu_consultas();
                break;
            case 4:
                menu_configuracion();
                break;
            case 5:
                printf("\nGracias por usar el sistema. ¡Hasta luego!\n");
                salir = 1;
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
        
        if (!salir) {
            pausa();
        }
    }
    
    return 0;
}

void mostrar_menu_principal(void) {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║     SISTEMA DE CONTROL DE TEMPERATURA - INVERNADERO     ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("1. Zonas\n");
    printf("2. Control de Temperaturas\n");
    printf("3. Consultas\n");
    printf("4. Configuracion\n");
    printf("5. Salir\n");
    printf("\n");
}

void menu_zonas(void) {
    int salir = 0;
    
    while (!salir) {
        printf("\n");
        printf("╔═══════════════════════════════════════╗\n");
        printf("║              MENU ZONAS               ║\n");
        printf("╚═══════════════════════════════════════╝\n");
        printf("\n");
        printf("a. Registro de Zona\n");
        printf("b. Volver al menu principal\n");
        printf("\n");
        printf("Seleccione una opcion: ");
        
        char opcion_char;
        if (scanf(" %c", &opcion_char) != 1) {
            printf("Error: Opcion invalida.\n");
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();
        
        switch (opcion_char) {
            case 'a':
            case 'A':
                registrar_zona();
                break;
            case 'b':
            case 'B':
                salir = 1;
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    }
}

void menu_control_temperaturas(void) {
    int salir = 0;
    
    while (!salir) {
        printf("\n");
        printf("╔════════════════════════════════════════════════╗\n");
        printf("║        CONTROL DE TEMPERATURAS                ║\n");
        printf("╚════════════════════════════════════════════════╝\n");
        printf("\n");
        printf("a. Ver temperatura actual\n");
        printf("b. Activar ventilador manualmente\n");
        printf("c. Ver historial de eventos\n");
        printf("d. Simular monitoreo en tiempo real\n");
        printf("e. Volver al menu principal\n");
        printf("\n");
        printf("Seleccione una opcion: ");
        
        char opcion_char;
        if (scanf(" %c", &opcion_char) != 1) {
            printf("Error: Opcion invalida.\n");
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();
        
        switch (opcion_char) {
            case 'a':
            case 'A':
                ver_temperatura_actual();
                break;
            case 'b':
            case 'B':
                activar_ventilador_manual();
                break;
            case 'c':
            case 'C':
                mostrar_historial_eventos();
                break;
            case 'd':
            case 'D':
                simular_monitoreo_tiempo_real();
                break;
            case 'e':
            case 'E':
                salir = 1;
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    }
}

void menu_consultas(void) {
    int salir = 0;
    
    while (!salir) {
        printf("\n");
        printf("╔═══════════════════════════════════════╗\n");
        printf("║            MENU CONSULTAS             ║\n");
        printf("╚═══════════════════════════════════════╝\n");
        printf("\n");
        printf("a. Buscar eventos por rango de temperatura o fecha\n");
        printf("b. Generar reporte estadistico\n");
        printf("c. Exportar historial a archivo CSV\n");
        printf("d. Volver al menu principal\n");
        printf("\n");
        printf("Seleccione una opcion: ");
        
        char opcion_char;
        if (scanf(" %c", &opcion_char) != 1) {
            printf("Error: Opcion invalida.\n");
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();
        
        switch (opcion_char) {
            case 'a':
            case 'A':
                buscar_eventos_por_filtro();
                break;
            case 'b':
            case 'B':
                generar_reporte_estadistico();
                break;
            case 'c':
            case 'C':
                exportar_historial_csv();
                break;
            case 'd':
            case 'D':
                salir = 1;
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    }
}

void menu_configuracion(void) {
    int salir = 0;
    
    while (!salir) {
        printf("\n");
        printf("╔═══════════════════════════════════════╗\n");
        printf("║         MENU CONFIGURACION            ║\n");
        printf("╚═══════════════════════════════════════╝\n");
        printf("\n");
        printf("a. Configurar umbral de temperatura por zona\n");
        printf("b. Restaurar configuracion por defecto por zona\n");
        printf("c. Volver al menu principal\n");
        printf("\n");
        printf("Seleccione una opcion: ");
        
        char opcion_char;
        if (scanf(" %c", &opcion_char) != 1) {
            printf("Error: Opcion invalida.\n");
            limpiar_buffer();
            continue;
        }
        limpiar_buffer();
        
        switch (opcion_char) {
            case 'a':
            case 'A':
                configurar_umbral_zona();
                break;
            case 'b':
            case 'B':
                restaurar_configuracion_default();
                break;
            case 'c':
            case 'C':
                salir = 1;
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    }
}

