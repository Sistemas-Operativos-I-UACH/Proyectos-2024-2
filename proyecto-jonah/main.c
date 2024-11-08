/*
Universidad Autonoma de Chihuahua
Materia: Sistemas Operativos
Alumno: Jonahtan Gabriel González Cardoza
Matricula: 345491

*/

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int main() {
    struct dirent *entry;
    DIR *pDir;
    char proc_dir[] = "/proc";
    char proc_id_dir[4096];
    char line[256];   
    char line2[256]; 
    FILE *fp;
    FILE *fp2;

    pDir = opendir(proc_dir);
    if (pDir == NULL) {
        printf("No se puede abrir el directorio '%s'\n", proc_dir);
        return 1;
    }

        printf("\n-----PROCESOS-----\n\n");
        while ((entry = readdir(pDir)) != NULL) {
            if(atoi(entry->d_name)){

                    sprintf(proc_id_dir, "/proc/%s/status", entry->d_name);
                    fp = fopen(proc_id_dir, "r");
                    if (fp == NULL) {
                        printf("No se pudo abrir el archivo.\n");
                        return 1;
                    }
                    
                    int line_count=0;
                    while (fgets(line, sizeof(line), fp)) {
                        line_count++;
                        if (line_count == 7) {
                            break;
                        }
                    }
                    int uid1, uid2, uid3, uid4;
                    if (fgets(line, sizeof(line), fp)) {
                        sprintf(proc_id_dir, "/proc/%s/comm", entry->d_name);
                        fp2 = fopen(proc_id_dir, "r");
                        fgets(line2, sizeof(line2), fp2);
                        if (fgets(line, sizeof(line), fp)) {
                            sscanf(line, "Uid: %d %d %d %d", &uid1, &uid2, &uid3, &uid4);
                            if (uid1==0){
                                printf("Proceso del KERNEL %s: %s", entry->d_name,line2);
                            }else{
                                printf("Proceso del USUARIO %s: %s", entry->d_name,line2);
                            }
                        }
                        fclose(fp2);
                    }

            fclose(fp);
            }

        }

    closedir(pDir);
    return 0;
}
