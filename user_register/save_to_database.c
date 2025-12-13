void save_to_database(char username[], char password[]) {
  File *database
  database=fopen("database.txt", "a")
  if(database==NULL){
  return 0;
  }
fprintf(fp,"name%s\n",username);
fprintf(fp,"password=%s\n",password);
fprintf(fp,"friends=\n");
fprintf(fp,"incoming_request=\n");
fprintf(fp,"outgoing_request=\n");
fprintf(database, "\n");
fclose(database);
 return 0;
}
