// go do this part charles

#include <stdio.h>
void manage_messages(char username[]) {
	scanf("%d", &message_choice);
    if (message_choice == 1) {
        // add friends
        send_messages(username);
        return 0;
    }
    else if (message_choice == 2) {
        // accept friends
        read_messages(username);
        return 0;
    }
    else if (message_choice == 3) {
        // delete friends
        delete_messages(username);
        return 0;
    }
    else if (message_choice == 4)
        // return to main.c
        return 0;
    else
        printf("Invalid choice! Please enter a number between 1 and 4.\n");
        return 0;
	
}