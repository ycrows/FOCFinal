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

    char line[256];
    int user_found = 0;
    char incoming_requests[10][50]; // max 10 requests, each max 50 chars
    int request_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        // check for "name=" line
        if (strncmp(line, "name=", 5) == 0) {
            char file_username[50];
            sscanf(line, "name=%49s", file_username);

            if (strcmp(file_username, username) == 0) { // when user is found
                user_found = 1;

                // skip password and friends line
                fgets(line, sizeof(line), fp); 
                fgets(line, sizeof(line), fp); 

                // read incoming_request line
                fgets(line, sizeof(line), fp);
                char *ptr = line + strlen("incoming_request="); // move past prefix
                ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                // split by comma
                char *token = strtok(ptr, ",");
                while (token != NULL) {
                    strcpy(incoming_requests[request_count++], token);
                    token = strtok(NULL, ",");
                }
                break;
            }
        }
    }

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
            printf("%d. %s\n", i+1, incoming_requests[i]);
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
                        // when name line match move incoming_requests to friends
                        i+=2; // move to incoming_requests line
                        char *friends_line = file_lines[i];
                        char *ptr = friends_line + strlen("friends=");
                        ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                        // move each request
                        for (int j = 0; j < request_count; j++) {
                            if (strlen(ptr) > 0) {
                                strcat(ptr, ",");
                            }
                            strcat(ptr, incoming_requests[j]);
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
                // iterate through entire incoming_requests and update
                char *sender_name = incoming_requests[k];
                for (int i = 0; i < line_count; i++) {
                    if (strncmp(file_lines[i], "name=", 5) == 0) {
                        // find sender block
                        char check_name[50];
                        sscanf(file_lines[i], "name=%49s", check_name);
                        if (strcmp(check_name, sender_name) == 0) {
                            // when name line match add user to sender's friends
                            i+=2; // move to incoming_requests 
                            char *inc_ptr = file_lines[i] + strlen("friends=");
                            inc_ptr[strcspn(inc_ptr, "\n")] = '\0';
                            if (strlen(inc_ptr) > 0) {
                                strcat(inc_ptr, ",");
                            }
                            strcat(inc_ptr, username); // add user
                            strcat(file_lines[i], "\n");

                            // remove user from sender's outgoing_request
                            i+=2; // move to outgoing_requests
                            char *out_ptr = file_lines[i] + strlen("outgoing_request=");
                            out_ptr[strcspn(out_ptr, "\n")] = '\0'; // remove newline

                            // rebuild the outgoing list without user
                            char new_out[256] = "outgoing_request=";
                            char *token = strtok(out_ptr, ",");
                            int first = 1; // for the first name
                            
                            while (token != NULL) {
                                // if token is not user, keep it
                                if (strcmp(token, username) != 0) {
                                    if (first == 0) {
                                        strcat(new_out, ",");
                                    }
                                    strcat(new_out, token);
                                    first = 0;
                                }
                                token = strtok(NULL, ",");
                            }
                            strcat(new_out, "\n");
                            strcpy(file_lines[i], new_out);
                            
                            break; // move to next sender
                        }
                    }
                }
            }

            // write to file
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
            char file_lines[1000][256]; // array to store lines
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
                        // when name line match move incoming_requests to friends
                        i+=2; // move to incoming_requests line
                        char *friends_line = file_lines[i];
                        char *ptr = friends_line + strlen("friends=");
                        ptr[strcspn(ptr, "\n")] = '\0'; // remove newline

                        // move each request
                        for (int j = 0; j < request_count; j++) {
                            if (strlen(ptr) > 0) {
                                strcat(ptr, ",");
                            }
                            strcat(ptr, incoming_requests[j]);
                        }
                        // rebuild incoming_request
                        i++; // move to incoming_request line
                        char *in_ptr = file_lines[i] + strlen("incoming_request=");
                        in_ptr[strcspn(in_ptr, "\n")] = '\0';
                        char new_requests[256] = "incoming_request=";
                        char *token = strtok(in_ptr, ",");
                        int first = 1; // for the first name

                        while (token != NULL) {
                            if (strcmp(token, incoming_requests[friend_choice-1]) != 0) {
                                if (first == 0) {
                                    strcat(new_requests, ",");
                                }
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

            // update sender block
            for (int i = 0; i < line_count; i++) {
                if (strncmp(file_lines[i], "name=", 5) == 0) {
                    char file_username[50];
                    sscanf(file_lines[i], "name=%49s", file_username);
                    if (strcmp(file_username, incoming_requests[friend_choice-1]) == 0) {
                        // when sender name founded 
                        i+=2;
                        char *friends_ptr = file_lines[i] + strlen("friends=");
                        friends_ptr[strcspn(friends_ptr, "\n")] = '\0';

                        if (strlen(friends_ptr) > 0) {
                            strcat(friends_ptr, ",");
                        }
                        strcat(friends_ptr, username);
                        strcat(file_lines[i], "\n");

                        // remove user from outgoing_request
                        i+=2; 
                        char *out_ptr = file_lines[i] + strlen("outgoing_request=");
                        out_ptr[strcspn(out_ptr, "\n")] = '\0';

                        char new_out[256] = "outgoing_request=";
                        char *token = strtok(out_ptr, ",");
                        int first = 1;

                        while (token != NULL) {
                            if (strcmp(token, username) != 0) {
                                if (first != 0) strcat(new_out, ",");
                                strcat(new_out, token);
                                first = 0;
                            }
                            token = strtok(NULL, ",");
                        }

                        strcat(new_out, "\n");
                        strcpy(file_lines[i], new_out);
                        break;
                    }
                }
            }

            // write to file
            FILE *fp_write = fopen("database.txt", "w");            
            for (int i = 0; i < line_count; i++) {
                fputs(file_lines[i], fp_write);
            }
            fclose(fp_write);
            printf("Friend requests updated for %s.\n", incoming_requests[friend_choice-1]);
        }
        tok = my_strtok_r(NULL, " ", &saveptr1);
    }
}

