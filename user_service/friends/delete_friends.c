#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "friends_func.h"

void remove_name_from_friends(char *friends_line, const char *name) {
    char buffer[256] = "friends=";
    char temp[256];
    strcpy(temp, friends_line + strlen("friends="));
    temp[strcspn(temp, "\n")] = '\0';

    char *token = strtok(temp, ",");
    int first = 1;

    while (token) {
        if (strcmp(token, name) != 0) {
            if (!first) strcat(buffer, ",");
            strcat(buffer, token);
            first = 0;
        }
        token = strtok(NULL, ",");
    }

    strcat(buffer, "\n");
    strcpy(friends_line, buffer);
}


void delete_friends(char username[]){
    FILE *fp = fopen("database.txt", "r");
    
    char line[256];
    int user_found = 0;
    char delete_lists[10][50]; // max 10 requests, each max 50 chars
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

                // Read delete_list line
                fgets(line, sizeof(line), fp);
                char *ptr = line + strlen("friends="); // move past the prefix
                ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                // Split by comma
                char *token = strtok(ptr, ",");
                while (token != NULL) {
                    strcpy(delete_lists[request_count++], token);
                    token = strtok(NULL, ",");
                }

                break;
            }
        }
    }

    if (!user_found) {
        printf("User not found.\n");
        return;
    }
    fclose(fp);
    if (request_count == 0) {
        printf("You have no friends.\n");
        return;
    } else {
        printf("Your friends:\n");
        for (int i = 0; i < request_count; i++) {
            printf("%d. %s\n", i+1, delete_lists[i]);
        }
        printf("%d. All\n", request_count+1);
        printf("%d. Back\n", request_count+2);
    }

    printf("Enter friend numbers (separated by space), press Enter to finish: ");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char input[256];
    int friend_choice;
    fgets(input, sizeof(input), stdin);  // read the whole line
    char *saveptr1;
    char *tok = my_strtok_r(input, " ", &saveptr1);

    while (tok != NULL) {
        friend_choice = atoi(tok);
        
        if (friend_choice == request_count+1) { 
            // remove all friends in user friend list
            // remove user from other friend list
            FILE *fp_read = fopen("database.txt", "r");
            char file_lines[1000][256];
            int line_count = 0;
            while (fgets(file_lines[line_count], sizeof(file_lines[line_count]), fp_read)) {
                line_count++;
            }
            fclose(fp_read);

            // Find user and clear friends
            for (int i = 0; i < line_count; i++) {
                if (strncmp(file_lines[i], "name=", 5) == 0) {
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);

                    if (strcmp(file_username, username) == 0) {
                        i += 2; // move to incoming_request line
                        strcpy(file_lines[i], "friends=\n");
                        break;
                    }
                }
            }

            for (int f = 0; f < request_count; f++) {
                for (int i = 0; i < line_count; i++) {
                    if (strncmp(file_lines[i], "name=", 5) == 0) {
                        char file_username[50];
                        sscanf(file_lines[i], "name=%49s", file_username);

                        if (strcmp(file_username, delete_lists[f]) == 0) {
                            i += 2; // friends line
                            remove_name_from_friends(file_lines[i], username);
                            break;
                        }
                    }
                }
            }

            FILE *fp_write = fopen("database.txt", "w");
            for (int i = 0; i < line_count; i++) {
                fputs(file_lines[i], fp_write);
            }
            fclose(fp_write);
            printf("Deleting all...\n");
            break;
        }
        else if (friend_choice == request_count+2) { 
            // return to the menu 
            return;
        } 
        else if (1 <= friend_choice && friend_choice <= request_count) { 
            // get the name from the array and based on friend_choice 
            // remove the name from the friend_list and remove user name from other user friend list
            char target[50];
            strcpy(target, delete_lists[friend_choice - 1]);

            FILE *fp_read = fopen("database.txt", "r");
            char file_lines[1000][256];
            int line_count = 0;

            while (fgets(file_lines[line_count], sizeof(file_lines[line_count]), fp_read)) {
                line_count++;
            }
            fclose(fp_read);

            /* Remove target from user's friends */
            for (int i = 0; i < line_count; i++) {
                if (strncmp(file_lines[i], "name=", 5) == 0) {
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);

                    if (strcmp(file_username, username) == 0) {
                        i += 2; // friends line
                        remove_name_from_friends(file_lines[i], target);
                        break;
                    }
                }
            }

            /* Remove user from target's friends */
            for (int i = 0; i < line_count; i++) {
                if (strncmp(file_lines[i], "name=", 5) == 0) {
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);

                    if (strcmp(file_username, target) == 0) {
                        i += 2; // friends line
                        remove_name_from_friends(file_lines[i], username);
                        break;
                    }
                }
            }

            FILE *fp_write = fopen("database.txt", "w");
            for (int i = 0; i < line_count; i++) {
                fputs(file_lines[i], fp_write);
            }
            fclose(fp_write);

            printf("Deleting %s...\n", target);
            
        }
        tok = my_strtok_r(NULL, " ", &saveptr1);
    } 
    printf("Friend list updated.\n");
    return;
}

