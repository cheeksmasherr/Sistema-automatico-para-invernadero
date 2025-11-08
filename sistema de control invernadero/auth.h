#ifndef AUTH_H
#define AUTH_H

#include "utilidades.h"


int validar_usuario(const char* nombre_usuario, const char* password);
int registrar_usuario(const char* nombre_usuario, const char* password);
void inicializar_archivo_usuarios(void);
int login(void);

#endif

