#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void request_builder(FILE *fp, char username[], char request_list[][50], int *request_count, int *user_found, int mode) {
    char line[256];
    
    while (fgets(line, sizeof(line), fp)) {
        // check for "name=" line
        if (strncmp(line, "name=", 5) == 0) {
            char file_username[50];
            sscanf(line, "name=%49s", file_username);

            if (strcmp(file_username, username) == 0) { // when user is found
                *user_found = 1;
                            
                fgets(line, sizeof(line), fp); // password line
                char *ptr;
                if (mode == 1) {
                    printf("hit1\n");
                    fgets(line, sizeof(line), fp); // friends line
                    fgets(line, sizeof(line), fp); 
                    puts(line);
                    printf("\n");
                    ptr = line + strlen("incoming_request="); // move past prefix
                }
                else if (mode == 2) {
                    printf("hit2\n");
                    fgets(line, sizeof(line), fp); 
                    puts(line);
                    printf("\n");
                    ptr = line + strlen("friends=");
                }
                ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                // split by comma
                char *token = strtok(ptr, ",");
                while (token != NULL) {
                    strcpy(request_list[*request_count], token);
                    (*request_count)++;
                    token = strtok(NULL, ",");
                }
                break;
            }
        }
    }
}