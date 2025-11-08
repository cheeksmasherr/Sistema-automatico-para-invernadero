#include "auth.h"

void inicializar_archivo_usuarios(void) {
    FILE* archivo = fopen(ARCHIVO_USUARIOS, "rb");
    if (archivo == NULL) {
        // Crear archivo con usuario por defecto
        archivo = fopen(ARCHIVO_USUARIOS, "wb");
        if (archivo != NULL) {
            Usuario usuario_default;
            strcpy(usuario_default.nombre_usuario, "admin");
            strcpy(usuario_default.password, "admin123");
            fwrite(&usuario_default, sizeof(Usuario), 1, archivo);
            fclose(archivo);
            printf("Archivo de usuarios inicializado. Usuario por defecto: admin / admin123\n");
        }
    } else {
        fclose(archivo);
    }
}

int validar_usuario(const char* nombre_usuario, const char* password) {
    FILE* archivo = fopen(ARCHIVO_USUARIOS, "rb");
    if (archivo == NULL) {
        return 0;
    }
    
    Usuario usuario;
    int encontrado = 0;
    
    while (fread(&usuario, sizeof(Usuario), 1, archivo) == 1) {
        if (strcmp(usuario.nombre_usuario, nombre_usuario) == 0 &&
            strcmp(usuario.password, password) == 0) {
            encontrado = 1;
            break;
        }
    }
    
    fclose(archivo);
    return encontrado;
}

int registrar_usuario(const char* nombre_usuario, const char* password) {
    if (!validar_string_no_vacio(nombre_usuario) || !validar_string_no_vacio(password)) {
        printf("Error: Nombre de usuario y contraseña no pueden estar vacios.\n");
        return 0;
    }
    
    FILE* archivo = fopen(ARCHIVO_USUARIOS, "ab");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo de usuarios.\n");
        return 0;
    }
    
    Usuario nuevo_usuario;
    strcpy(nuevo_usuario.nombre_usuario, nombre_usuario);
    strcpy(nuevo_usuario.password, password);
    
    fwrite(&nuevo_usuario, sizeof(Usuario), 1, archivo);
    fclose(archivo);
    
    printf("Usuario registrado exitosamente.\n");
    return 1;
}

int login(void) {
    char nombre_usuario[MAX_NOMBRE];
    char password[MAX_PASSWORD];
    int intentos = 0;
    const int max_intentos = 3;
    
    printf("\n=== SISTEMA DE CONTROL DE TEMPERATURA ===\n");
    printf("=== INICIO DE SESION ===\n\n");
    
    while (intentos < max_intentos) {
        printf("Nombre de usuario: ");
        if (fgets(nombre_usuario, sizeof(nombre_usuario), stdin) == NULL) {
            continue;
        }
        nombre_usuario[strcspn(nombre_usuario, "\n")] = '\0';
        
        printf("Contraseña: ");
        if (fgets(password, sizeof(password), stdin) == NULL) {
            continue;
        }
        password[strcspn(password, "\n")] = '\0';
        
        if (validar_usuario(nombre_usuario, password)) {
            printf("\n¡Bienvenido, %s!\n", nombre_usuario);
            return 1;
        } else {
            intentos++;
            printf("Credenciales incorrectas. Intentos restantes: %d\n\n", max_intentos - intentos);
        }
    }
    
    printf("Se agotaron los intentos. Acceso denegado.\n");
    return 0;
}

