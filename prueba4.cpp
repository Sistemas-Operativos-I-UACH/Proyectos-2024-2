
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> //Permite manipular directorios, para leer el contenido de /proc
#include <ctype.h> //Contiene funciones para comprobar y convertir caracteres como isdigit
#include <string.h>

// Funciones
//Se encarga de leer el archivo cmdline de un proceso y devuelve si es proceso de kernel o de usuario
char fetch_process(char *filename, char *procname);

void fetch_kernel_process_name(char *filename, char *procname);

int main(int argc, char *argv[]) {
    // Inicialización de variables
    struct dirent *entry;
//DIR es una estructura utilizada para representar un directorio abierto. En este caso, pDir se usa para almacenar el resultado de abrir el directorio /proc y luego recorrer su contenido.
    DIR *pDir;
    char proc_dir[] = "/proc";
    char proc_id_dir[1024];
    char cmdline[1024];
    char isKernel;

    // Abre el directorio /proc
    pDir = opendir(proc_dir);
    if(pDir == NULL) {
        printf("No se puede abrir el directorio '%s'\n", proc_dir);
        return 1;
    }

    // Recorre cada entrada en el directorio /proc
    while((entry = readdir(pDir)) != NULL) {
        // Solo procesa directorios cuyo primer carácter es un dígito
        if (!isdigit(entry->d_name[0]))
            continue;

        // Obtiene la ruta al archivo cmdline del proceso
        sprintf(proc_id_dir, "/proc/%s/cmdline", entry->d_name);	
        // Obtiene el nombre del proceso y determina si es de usuario o de kernel
        isKernel = fetch_process(proc_id_dir, cmdline);

        // Si es un proceso de kernel, obtiene su nombre del archivo 'comm'
        if (isKernel == 'K') {
            sprintf(proc_id_dir, "/proc/%s/comm", entry->d_name);
            fetch_kernel_process_name(proc_id_dir, cmdline);
            printf("[K] %s:%s", entry->d_name, cmdline);
        } else {
            // Imprime el nombre del proceso de usuario
            printf("[U] %s:%s\n", entry->d_name, cmdline);
        }
    }

    closedir(pDir);
    printf("\n[U]ser process | [K]ernel process\n");
    return 0;
}

char fetch_process(char *filename, char *procname) {
    // Función para obtener el nombre del proceso
    FILE *file_pointer;
    char *line = NULL;
    size_t len = 0;
    procname[0] = '\0';

    // Abre el archivo cmdline del proceso
    if (filename)
        file_pointer = fopen(filename, "r");

    // Verifica si el archivo se abrió correctamente
    if (file_pointer == NULL) {
        printf("Error while opening file '%s'\nExit code 2\n", filename);
        exit(2);
    }

    // Lee el contenido del archivo línea por línea
    while (getline(&line, &len, file_pointer) != -1) {
        // Concatena el contenido a procname
        sprintf(procname, "%s%s", procname, line);
    }

    fclose(file_pointer);
    free(line);

    // Si procname está vacío, el proceso es del kernel; de lo contrario, es un proceso de usuario
    if (procname[0] == '\0')
        return 'K';
    else
        return 'U';
}

void fetch_kernel_process_name(char *filename, char *procname) {
    // Función para obtener el nombre del proceso de kernel desde /proc/[pid]/comm
    FILE *file_pointer;
    char *line = NULL;
    size_t len = 0;
    procname[0] = '\0';

    // Abre el archivo comm del proceso
    if (filename)
        file_pointer = fopen(filename, "r");

    // Verifica si el archivo se abrió correctamente
    if (file_pointer == NULL) {
        printf("Error while opening file '%s'\nExit code 2\n", filename);
        exit(2);
    }

    // Lee el contenido del archivo línea por línea (normalmente solo contiene una línea)
    if (getline(&line, &len, file_pointer) != -1) {
        // Copia el contenido a procname
        sprintf(procname, "%s", line);
    }

    fclose(file_pointer);
    free(line);
}
