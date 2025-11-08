#ifndef TEMPERATURA_H
#define TEMPERATURA_H

#include "utilidades.h"


float leer_temperatura_simulada(void);
void ver_temperatura_actual(void);
void activar_ventilador_manual(void);
int controlar_ventilador_automatico(int id_zona, float temperatura);
void simular_monitoreo_tiempo_real(void);

#endif

