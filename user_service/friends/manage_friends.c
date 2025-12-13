#include <stdio.h>
void manage_friends(char username[]) {
	int friends_choice = 0;
  	printf("==================== Main Service ====================\n");
  	printf("1. Add friends\n");
  	printf("2. Accept friends\n");
	printf("3. Delete friends\n");
    printf("4. Show current friends\n");
    printf("5. Back\n");
	printf("===============================================\n");
	printf("Choose an option (1-5): 1");
	
    scanf("%d", &friends_choice);
    if (friends_choice == 1) {
        // add friends
        add_friends(username);
        return 0;
    }
    else if (friends_choice == 2) {
        // accept friends
        accept_friends(username);
        return 0;
    }
    else if (friends_choice == 3) {
        // delete friends
        delete_friends(username);
        return 0;
    }
    else if (friends_choice == 4) {
        // show current friends
        show_friends(username);
        return 0;
    }
    else if (friends_choice == 5)
        // return to main.c
        return 0;
    else
        printf("Invalid choice! Please enter a number between 1 and 5.\n");
        return 0;
	
}