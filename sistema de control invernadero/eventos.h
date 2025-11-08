#ifndef EVENTOS_H
#define EVENTOS_H

#include "utilidades.h"

void registrar_evento(int id_zona, float temperatura, int estado_ventilador);
NodoEvento* cargar_eventos(void);
void liberar_lista_eventos(NodoEvento* lista);
void mostrar_historial_eventos(void);
int guardar_evento_en_archivo(Evento* evento);

#endif

