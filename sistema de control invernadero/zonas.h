#ifndef ZONAS_H
#define ZONAS_H

#include "utilidades.h"

// Prototipos de zonas
int cargar_zonas(Zona** zonas, int* num_zonas);
int guardar_zonas(Zona* zonas, int num_zonas);
int registrar_zona(void);
void mostrar_zonas(Zona* zonas, int num_zonas);
int obtener_siguiente_id_zona(void);
int validar_id_unico(int id);

#endif

