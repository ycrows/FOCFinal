#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "friends_func.h"

void accept_friends(char username[]){
    FILE *fp = fopen("database.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
        return; 
    }

    int user_found = 0;
    char request_list[10][50]; // max 10 requests, each max 50 chars
    int request_count = 0;
    int mode = 1;

    request_builder(fp, username, request_list, &request_count, &user_found, mode);
   
    // display output
    if (user_found == 0) {
        printf("User not found.\n");
        return;
    }
    fclose(fp);
    if (request_count == 0) {
        printf("No pending friend requests for %s.\n", username);
        return;
    } 
    else {
        printf("Pending friend requests for %s:\n", username);
        for (int i = 0; i < request_count; i++) {
            printf("%d. %s\n", i+1, request_list[i]);
        }
        printf("%d. All\n", request_count+1);
        printf("%d. Back\n", request_count+2);
    }

    // get user input
    printf("Enter indices (space separated), press Enter to finish: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char input[256];
    int friend_choice;
    fgets(input, sizeof(input), stdin);  // read the whole line
    char *saveptr1;
    char *tok = my_strtok_r(input, " ", &saveptr1); // split the user input
    
    while (tok != NULL) {
        friend_choice = atoi(tok); // convert string from token to int

        if (friend_choice == request_count+1) {
            FILE *fp_read = fopen("database.txt", "r"); 
            char file_lines[1000][256]; // array to store the lines
            int line_count = 0;
            while (fgets(file_lines[line_count], sizeof(file_lines[line_count]), fp_read)) {
                line_count++;
            }
            fclose(fp_read);
            
            // update the user's block
            for (int i = 0; i < line_count; i++) {
                // find name line
                if (strncmp(file_lines[i], "name=", 5) == 0) { 
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);
                    if (strcmp(file_username, username) == 0) {
                        // when name line match move request_list to friends
                        i+=2; // move to request_list line
                        char *friends_line = file_lines[i];
                        char *ptr = friends_line + strlen("friends=");
                        ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                        // move each request
                        for (int j = 0; j < request_count; j++) {
                            if (strlen(ptr) > 0) {
                                strcat(ptr, ",");
                            }
                            strcat(ptr, request_list[j]);
                        }

                        // clear incoming_request
                        i++; // move to incoming_request line
                        strcpy(file_lines[i], "\nincoming_request=\n");
                        break;
                    }
                }
            }
            
            // update the sender's block
            for (int k = 0; k < request_count; k++) {
                // iterate through entire request_list and update
                char *sender_name = request_list[k];
                for (int i = 0; i < line_count; i++) {
                    if (strncmp(file_lines[i], "name=", 5) == 0) {
                        // find sender block
                        char check_name[50];
                        sscanf(file_lines[i], "name=%49s", check_name);
                        if (strcmp(check_name, sender_name) == 0) {
                            // when name line match add user to sender's friends
                            i+=2; // move to request_list 
                            char *f_ptr = file_lines[i] + strlen("friends=");
                            f_ptr[strcspn(f_ptr, "\n")] = '\0';
                            if (strlen(f_ptr) > 0) {
                                strcat(f_ptr, ",");
                            }
                            strcat(f_ptr, username); // add user
                            strcat(file_lines[i], "\n");

                            // remove user from sender's outgoing_request
                            i+=2; // move to outgoing_requests
                            char *out_ptr = file_lines[i] + strlen("outgoing_request=");
                            out_ptr[strcspn(out_ptr, "\n")] = '\0';

                            // Rebuild the outgoing list without your name
                            char new_out[256] = "outgoing_request=";
                            char *tok = strtok(out_ptr, ",");
                            int first = 1;
                            
                            while (tok) {
                                // If token is not user, keep it
                                if (strcmp(tok, username) != 0) {
                                    if (!first) strcat(new_out, ",");
                                    strcat(new_out, tok);
                                    first = 0;
                                }
                                tok = strtok(NULL, ",");
                            }
                            strcat(new_out, "\n");
                            strcpy(file_lines[i], new_out);
                            
                            break; // done with this sender, move to the next one
                        }
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
            return;
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
                        // Append request_list to friends
                        i++; // password line
                        i++; // friends line
                        char *friends_line = file_lines[i];
                        char *ptr = friends_line + strlen("friends=");
                        ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                        if (strlen(ptr) > 0) {
                                strcat(ptr, ",");
                        }
                        strcat(ptr, request_list[friend_choice-1]);
                        strcat(file_lines[i], "\n"); // ensure newline

                        // Step 3: Clear incoming_request
                        i++; // move to incoming_request line
                        char *in_ptr = file_lines[i] + strlen("incoming_request=");
                        in_ptr[strcspn(in_ptr, "\n")] = '\0';
                        char new_requests[256] = "incoming_request=";
                        char *token = strtok(in_ptr, ",");
                        int first = 1;
                        while (token != NULL) {
                            if (strcmp(token, request_list[friend_choice-1]) != 0) {
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

            for (int i = 0; i < line_count; i++) {
                if (strncmp(file_lines[i], "name=", 5) == 0) {
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);

                    if (strcmp(file_username, request_list[friend_choice-1]) == 0) {
                        // 1. add username to friends
                        i++; // password
                        i++; // friends
                        char *friends_ptr = file_lines[i] + strlen("friends=");
                        friends_ptr[strcspn(friends_ptr, "\n")] = '\0';

                        if (strlen(friends_ptr) > 0) strcat(friends_ptr, ",");
                        strcat(friends_ptr, username);
                        strcat(file_lines[i], "\n");

                        // 2. remove username from outgoing_request
                        i += 2; // incoming_request -> outgoing_request
                        char *out_ptr = file_lines[i] + strlen("outgoing_request=");
                        out_ptr[strcspn(out_ptr, "\n")] = '\0';

                        char new_out[256] = "outgoing_request=";
                        char *tok = strtok(out_ptr, ",");
                        int first = 1;

                        while (tok) {
                            if (strcmp(tok, username) != 0) {
                                if (!first) strcat(new_out, ",");
                                strcat(new_out, tok);
                                first = 0;
                            }
                            tok = strtok(NULL, ",");
                        }

                        strcat(new_out, "\n");
                        strcpy(file_lines[i], new_out);
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

            printf("Friend requests updated for %s.\n", request_list[friend_choice-1]);
        }
        tok = my_strtok_r(NULL, " ", &saveptr1);
    }
}

