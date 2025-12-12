#include <stdio.h>
#include <string.h>
int check_if_user_exists(char username[]) {
  //variables declaration
  int c;
  char name_in_list[50];
  //file open & check
  FILE *fp_database = fopen("../database.txt","r");
  if(fp_database == NULL){
    printf("Failed to open database.txt");
    return 0;
  };
  //initialize the cursor(to the 2nd row)
  if((c = fgetc(fp_database))==EOF){
    fclose(fp_database);
    return 0;
  }
  while((c = fgetc(fp_database)) != EOF && c != '\n');
  //move the cursor to the 2+7n line
  do{
    fseek(fp_database, 5, SEEK_CUR);
    if (fgets(name_in_list, sizeof(name_in_list), fp_database) == NULL) {
      fclose(fp_database);
      return 0;
      }
    name_in_list[strcspn(name_in_list, "\n")] = '\0';
    if (strncmp(name_in_list, username, strlen(username)) == 0) {
      fclose(fp_database);
      return 1;
      }
    for(int i = 0; i<6; i++){
      while((c = fgetc(fp_database)) != EOF && c != '\n');
    }
    c = fgetc(fp_database);
    if (c == EOF) {
      fclose(fp_database);
      return 0;
      }
    fseek(fp_database, -1, SEEK_CUR);
  }while(1);
  fclose(fp_database);
}
