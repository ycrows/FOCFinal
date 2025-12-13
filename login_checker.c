#include <stdio.h>
#include <string.h>
int login_checker(char username[], char password[]){
    
    FILE *fp = fopen("database.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    char line[256];
    char file_username[50];
    char file_password[50];
    int user_found = 0;

    while (fgets(line, sizeof(line), fp)) { // iterate through every line
        // find the names in database by searching for name=
        if (strncmp(line, "name=", 5) == 0) {
            sscanf(line, "name=%49s", file_username);

            // if username matches
            if (strcmp(file_username, username) == 0) {
                user_found = 1; // mark user found
                // read next line password=
                if (fgets(line, sizeof(line), fp)) {
                    sscanf(line, "password=%49s", file_password);
                    if (strcmp(file_password, password) == 0) {
                        printf("Login successful. Welcome %s!\n", username);
                        return 1;
                    } else {
                        printf("Warning! Incorrect password.\n");
                        return 0;
                    }
                }
                break;
            }
        }
    }

    if (!user_found) {
        printf("Warning! Account name not found.\n");
    }

    fclose(fp);
    return 0;
}