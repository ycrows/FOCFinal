#include <stdio.h>
#include "messages_func.h"

void manage_messages(char username[]) {
	int message_choice = 0;
    while (1) {
        printf("==================== Manage Messages ====================\n");
        printf("1. Send a message\n");
        printf("2. Read messages\n");
        printf("3. Delete messages\n");
        printf("4. Back\n");
        printf("=========================================================\n");
        printf("Choose an option (1-4): ");
        
        scanf("%d", &message_choice);
        if (message_choice == 1) {
            // add friends
            send_messages(username);
            continue;
        }
        else if (message_choice == 2) {
            // accept friends
            read_messages(username);
            continue;
        }
        else if (message_choice == 3) {
            // delete friends
            delete_messages(username);
            continue;
        }
        else if (message_choice == 4)
            // return to main.c
            break;
        else
            printf("Invalid choice! Please enter a number between 1 and 4.\n");
            break;
    }
}