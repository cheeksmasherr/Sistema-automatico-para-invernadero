# Sistema de Control de Temperatura - Invernadero Automatizado

## Descripción

Sistema modular en lenguaje C que simula el monitoreo y control de temperatura en un invernadero, aplicando conceptos fundamentales de programación estructurada: estructuras, punteros, memoria dinámica, manejo de archivos, modularidad y menús interactivos.

## Características

- Autenticación de usuarios con archivo binario
-  Registro y gestión de zonas del invernadero
-  Control automático y manual de ventiladores
-  Monitoreo de temperatura en tiempo real
-  Historial de eventos con fechas y horas
-  Búsqueda de eventos por filtros (temperatura, fecha)
-  Reportes estadísticos (máxima, mínima, promedio)
-  Exportación de datos a CSV
-  Configuración de umbrales por zona
-  Restauración de configuración por defecto

## Estructura del Proyecto

### Parte 1: Estructuras Base y Autenticación
- `utilidades.h/c` - Librería propia con validaciones, constantes y funciones auxiliares
- `auth.h/c` - Sistema de autenticación de usuarios
- `zonas.h/c` - Gestión de zonas del invernadero

### Parte 2: Control de Temperatura y Eventos
- `temperatura.h/c` - Control de temperatura y ventiladores
- `eventos.h/c` - Gestión de eventos e historial

### Parte 3: Consultas, Reportes y Configuración
- `reportes.h/c` - Consultas, búsquedas y reportes estadísticos
- `config.h/c` - Configuración del sistema
- `main.c` - Menú principal e integración de módulos

## Compilación

### Usando Makefile (Recomendado)
```bash
make          
make run      
make clean    
make clean-data  
make clean-all   
```

### Compilación Manual
```bash
gcc -Wall -Wextra -std=c11 -o sistema_invernadero main.c utilidades.c auth.c zonas.c temperatura.c eventos.c reportes.c config.c -lm
```

## Uso del Sistema

### Credenciales por Defecto
- Usuario: `admin`
- Contraseña: `admin123`

### Menú Principal

1. **Zonas**
   - Registro de Zona

2. **Control de Temperaturas**
   - Ver temperatura actual
   - Activar ventilador manualmente
   - Ver historial de eventos
   - Simular monitoreo en tiempo real

3. **Consultas**
   - Buscar eventos por rango de temperatura o fecha
   - Generar reporte estadístico
   - Exportar historial a archivo CSV

4. **Configuración**
   - Configurar umbral de temperatura por zona
   - Restaurar configuración por defecto por zona

5. **Salir**

## Archivos de Datos

El sistema genera y utiliza los siguientes archivos binarios:

- `usuarios.dat` - Usuarios del sistema
- `zonas.dat` - Zonas registradas
- `eventos.dat` - Historial de eventos

## Requisitos

- Compilador GCC o Clang
- Sistema operativo: macOS, Linux o Windows (con MinGW)
- Make (opcional, para usar el Makefile)

## Conceptos Implementados

Estructuras de datos
Punteros y aritmética de punteros
Memoria dinámica (malloc/free/realloc)
Manejo de archivos binarios y de texto
Listas dinámicas enlazadas
Validaciones de entrada
Menús interactivos con switch
Módulos y separación de responsabilidades
Librerías propias 
Comparación de cadenas y booleanos
Simulación de lectura de sensores

## Notas

- Las temperaturas se generan aleatoriamente entre 15°C y 35°C
- El umbral por defecto es 25°C
- Los eventos se registran automáticamente cuando cambia el estado del ventilador
- Los archivos CSV pueden abrirse directamente en Excel

## Autores

Pedro Ramses, Eric Nava, Rolando Obregon

