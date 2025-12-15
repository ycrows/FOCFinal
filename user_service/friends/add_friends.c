#include <stdio.h>
#include <string.h>
#include "../../user_register/user_register_func.h"

int add_friends(char sender_name[]){
    // get who to add from user (scan)
    char receiver_name[50];
    printf("Enter the username to add: ");
    scanf("%s", receiver_name);

    // check if user add self
    if (strcmp(sender_name, receiver_name) == 0) {
        printf("Warning! Cannot add yourself as a friend!\n");
        return -1;
    }

    // check if user add someone not in database
    if (check_if_user_exists(receiver_name) == 0) {
        printf("Warning! The target user does not exist! Add friend failed.\n");
        return -2;
    }

    // if user account exist then
    // 1. Check if already sent request (outgoing of sender has receiver)
    // 2. Check if other side sent request (incoming of sender has receiver)
    FILE *fp_db = fopen("../database.txt", "r");
    if (fp_db == NULL) {
        printf("Failed to open database.txt!\n");
        return 0;
    }
    char line[100];
    int has_sent = 0;  
    int other_sent = 0;
    // Loop read database to check request status
    while (fgets(line, 100, fp_db) != NULL) {
        // Check sender's outgoing_request
        if (strstr(line, "name=") != NULL && strstr(line, sender_name) != NULL) {
            while (fgets(line, 100, fp_db) != NULL) {
                if (strstr(line, "outgoing_request=") != NULL) {
                    if (strstr(line, receiver_name) != NULL) {
                        has_sent = 1;
                    }
                    break;
                }
            }
        }
        // Check receiver's outgoing_request (i.e., sender's incoming_request)
        if (strstr(line, "name=") != NULL && strstr(line, receiver_name) != NULL) {
            while (fgets(line, 100, fp_db) != NULL) {
                if (strstr(line, "outgoing_request=") != NULL) {
                    if (strstr(line, sender_name) != NULL) {
                        other_sent = 1;
                    }
                    break;
                }
            }
        }
    }
    fclose(fp_db);  // Close file after checking

    // Judge error cases
    if (has_sent == 1) {
        // if second time then --> output fail
        printf("Warning! Friend request already sent! Please wait for approval.\n");
        return -3;
    }
    if (other_sent == 1) {
        // if already in incoming --> output fail
        printf("Warning! The other party has sent you a friend request! Please agree first.\n");
        return -4;
    }

    // All checks passed, update database
    char db_content[100][100];
    int line_count = 0; 
    FILE *fp_read = fopen("../database.txt", "r");
    if (fp_read == NULL) {
        printf("Failed to open database.txt for reading!\n");
        return 0;
    }
    while (fgets(db_content[line_count], 100, fp_read) != NULL) {
        line_count++;
    }
    fclose(fp_read); 

    int i = 0;
    while (i < line_count) {
        if (strstr(db_content[i], "name=") != NULL && strstr(db_content[i], sender_name) != NULL) {
            for (int j = i + 1; j < line_count; j++) {
                if (strstr(db_content[j], "outgoing_request=") != NULL) {
                    sprintf(db_content[j], "outgoing_request=%s\n", receiver_name);
                    break; 
                }
            }
        }
        if (strstr(db_content[i], "name=") != NULL && strstr(db_content[i], receiver_name) != NULL) {
            for (int j = i + 1; j < line_count; j++) {
                if (strstr(db_content[j], "incoming_request=") != NULL) {
                    sprintf(db_content[j], "incoming_request=%s\n", sender_name);
                    break; 
                }
            }
        }
        i++;
    }
    FILE *fp_write = fopen("../database.txt", "w"); 
    if (fp_write == NULL) {
        printf("Failed to open database.txt for writing!\n");
        return 0;
    }
    for (int k = 0; k < line_count; k++) {
        fputs(db_content[k], fp_write);
    }
    fclose(fp_write);
    printf("Friend request sent successfully!\n");
    return 1;
}
