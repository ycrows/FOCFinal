#include <stdio.h>
#include <string.h>

void write(FILE *fp, char username[], char password[]) {
  fprintf(fp,"name=%s\n",username);
  fprintf(fp,"password=%s\n",password);
  fprintf(fp,"friends=\n");
  fprintf(fp,"incoming_request=\n");
  fprintf(fp,"outgoing_request=\n");
  fprintf(fp, "\n");
}

void save_to_database(char username[], char password[]) {
  FILE *fp=fopen("database.txt", "a");
  int c = fgetc(fp); // try reading the first character
  // empty file  
  if (c == EOF) {
      write(fp, username, password);
  } 
  // non empty file
  else {
      fprintf(fp, "\n");
      fprintf(fp, "\n");
      write(fp, username, password);
  }
  fclose(fp);
}