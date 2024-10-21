#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>

void get_process_cmd(char *filename, char *procname);

int main (int argc, char *argv[]) {
        struct dirent *entry;
        DIR *pDir;
        char proc_dir[] = "/proc";
        char proc_id_dir[255];
        char cmdline[255];

        /*if (argc != 2) {
                printf ("Usage: testprog <dirname>\n");
                return 1;
        }*/

        // Ensure we can open directory.

        pDir = opendir (proc_dir);
        if (pDir == NULL) {
                printf ("Cannot open directory UFF '%s'\n", argv[1]);
                return 1;
        }

        // Process each entry.

        while ((entry = readdir(pDir)) != NULL) {
        if ( ! isdigit(entry->d_name[0]) )
          continue;

            sprintf(proc_id_dir, "/proc/%s/cmdline", entry->d_name);
            get_process_cmd(proc_id_dir, cmdline);
            printf ("%s:%s\n", entry->d_name, cmdline);
        }

        // Close directory and exit.

        closedir (pDir);
        return 0;
}


void get_process_cmd(char *filename, char *procname) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  procname[0] = '\0';

  // Open file in read-only mode
  fp = fopen(filename,"r");

  // If file opened successfully, then print the contents
  if ( fp == NULL ) {
    printf("Error abriendo archivo %s\n", filename);
    exit(2);
   }

  while(getline(&line, &len, fp) != -1) {
    sprintf(procname, "%s%s", procname, line);
  }

  fclose(fp);
  free(line);
}
