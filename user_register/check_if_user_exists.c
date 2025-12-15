#include <stdio.h>
#include<string.h>
int check_if_user_exists(char username[]) {
  FILE *fp = fopen("../database.txt","r");
  if(fp == NULL){
    printf("Warning! file \"database.txt\" cannot be openned!\n");
    return 0;
  }
  char buf[1024];
  char target[50];
  sprintf(target,"name=%s",username);
  while(fgets(buf, sizeof(buf), fp) != NULL){
    if(strstr(buf, target) != NULL){
      fclose(fp);
      return 1;
    }
  }
  fclose(fp);
  return 0;
}
