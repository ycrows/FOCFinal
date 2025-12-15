#include <stdio.h>
#include <string.h>

int accept_friends(char username[]){
    FILE *fp = fopen("database.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    char username[50] = "charles";
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

    if (!user_found) {
        printf("User not found.\n");
        return 0;
    }
    fclose(fp);
    printf("Pending friend requests for %s:\n", username);
    if (request_count == 0) {
        printf("No incoming requests.\n");
    } else {
        for (int i = 0; i < request_count; i++) {
            printf("%d. %s\n", i+1, incoming_requests[i]);
        }
        printf("%d. All\n", request_count+1);
        printf("%d. Back\n", request_count+2);
    }

    printf("Enter indices (space separated), press Enter to finish: ");
    
    do {
        int friend_choice;
        scanf("%d", &friend_choice);
        if (friend_choice == request_count+1) {
            FILE *fp_read = fopen("database.txt", "r");
            char file_lines[1000][256]; // max 1000 lines
            int line_count = 0;
            while (fgets(file_lines[line_count], sizeof(file_lines[line_count]), fp_read)) {
                line_count++;
            }
            fclose(fp_read);
            
            // Step 2: Update the user's block
            for (int i = 0; i < line_count; i++) {
                if (strncmp(file_lines[i], "name=", 5) == 0) {
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);

                    if (strcmp(file_username, username) == 0) {
                        // Append incoming_requests to friends
                        i++; // password line
                        i++; // friends line
                        char *friends_line = file_lines[i];
                        char *ptr = friends_line + strlen("friends=");
                        ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                        // Append each request
                        for (int j = 0; j < request_count; j++) {
                            if (strlen(ptr) > 0) {
                                strcat(ptr, ",");
                            }
                            strcat(ptr, incoming_requests[j]);
                        }

                        // Step 3: Clear incoming_request
                        i++; // move to incoming_request line
                        strcpy(file_lines[i], "\nincoming_request=\n");

                        break;
                    }
                }
            }

            // Step 4: Write back to file
            FILE *fp_write = fopen("database.txt", "w");
            
            for (int i = 0; i < line_count; i++) {
                fputs(file_lines[i], fp_write);
            }
            fclose(fp_write);

            printf("Friend requests updated for all.\n");
            break;
        }
        else if (friend_choice == request_count+2) {
            // return to the menu
            break;
        }
        else if (1 <= friend_choice && friend_choice <= request_count) {
            // get the name from the array and based on friend_choice
            // remove the name from the incoming_request and add the name to friends
            FILE *fp_read = fopen("database.txt", "r");
            char file_lines[1000][256]; // max 1000 lines
            int line_count = 0;
            while (fgets(file_lines[line_count], sizeof(file_lines[line_count]), fp_read)) {
                line_count++;
            }
            fclose(fp_read);
            
            // Step 2: Update the user's block
            for (int i = 0; i < line_count; i++) {
                if (strncmp(file_lines[i], "name=", 5) == 0) {
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);

                    if (strcmp(file_username, username) == 0) {
                        // Append incoming_requests to friends
                        i++; // password line
                        i++; // friends line
                        char *friends_line = file_lines[i];
                        char *ptr = friends_line + strlen("friends=");
                        ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                        if (strlen(ptr) > 0) {
                                strcat(ptr, ",");
                        }
                        strcat(ptr, incoming_requests[friend_choice-1]);
                        strcat(file_lines[i], "\n"); // ensure newline

                        // Step 3: Clear incoming_request
                        i++; // move to incoming_request line
                        char *in_ptr = file_lines[i] + strlen("incoming_request=");
                        in_ptr[strcspn(in_ptr, "\n")] = '\0';
                        char new_requests[256] = "incoming_request=";
                        char *token = strtok(in_ptr, ",");
                        int first = 1;
                        while (token != NULL) {
                            if (strcmp(token, incoming_requests[friend_choice-1]) != 0) {
                                if (!first) strcat(new_requests, ",");
                                strcat(new_requests, token);
                                first = 0;
                            }
                            token = strtok(NULL, ",");
                        }
                        strcat(new_requests, "\n");
                        strcpy(file_lines[i], new_requests);
                        break;
                    }
                }
            }

            // Step 4: Write back to file
            FILE *fp_write = fopen("database.txt", "w");
            
            for (int i = 0; i < line_count; i++) {
                fputs(file_lines[i], fp_write);
            }
            fclose(fp_write);

            printf("Friend requests updated for %s.\n", incoming_requests[friend_choice-1]);
            break;
        }
    } while (1);
    
    return 0;
}