#include <stdio.h>
#include <string.h>

int accept_friends(char username[]){
    FILE *fp = fopen("database.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    char line[256];
    int user_found = 0;
    char incoming_requests[10][50]; // max 10 requests, each max 50 chars
    int request_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Check for "name=" line
        if (strncmp(line, "name=", 5) == 0) {
            char file_username[50];
            sscanf(line, "name=%49s", file_username);

            if (strcmp(file_username, username) == 0) {
                user_found = 1;

                // Skip next two lines (password and friends)
                fgets(line, sizeof(line), fp); // password line
                fgets(line, sizeof(line), fp); // friends line

                // Read incoming_request line
                fgets(line, sizeof(line), fp);
                char *ptr = line + strlen("incoming_request="); // move past the prefix
                ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                // Split by comma
                char *token = strtok(ptr, ",");
                while (token != NULL) {
                    strcpy(incoming_requests[request_count++], token);
                    token = strtok(NULL, ",");
                }

                break;
            }
        }
    }

    fclose(fp);

    if (!user_found) {
        printf("User not found.\n");
        return 0;
    }

    printf("Pending friend requests for %s:\n", username);
    if (request_count == 0) {
        printf("No incoming requests.\n");
    } else {
        int i;
        for (i = 0; i < request_count; i++) {
            printf("%d. %s\n", i+1, incoming_requests[i]);
        }
        
        printf("%d. All\n", i+1);
        printf("%d. Back\n", i+2);
    }

    

    return 0;
}