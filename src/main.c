#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "file.h"

void print_usage(char *argv[]) {
  printf("Uso: %s -n -f <database file>\n", argv[0]);
  printf("\t -n crea una nueva base de datos\n");
  printf("\t -f carga una base de datos, necesita el path\n");
  return;
}

int main(int argc, char *argv[]) {
  int dbfd;
  char *filename;
  bool newfile = false;

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "nf:")) != -1){
    switch (c) {
      case 'n': 
        newfile = true;
        break;
      case 'f': 
        filename = optarg;
        break;
      case '?': 
        //TODO: manejar errores concretos
        print_usage(argv);
        return -1;
      default:
        return -1;
    }
  }

  if (newfile) {
    dbfd = create_db_file(filename);
  }

  return 0;
}
