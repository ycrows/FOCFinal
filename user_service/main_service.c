#include <stdio.h>

void main_service(char username[]){
	int service_choice = 0;
  	while (1) {
		printf("==================== Main Service ====================\n");
		printf("1. Manage friends\n");
		printf("2. Manage messages\n");
		printf("3. Back\n");
		printf("===============================================\n");
		printf("Choose an option (1-3): ");
		scanf("%d", &service_choice);
		if (service_choice == 1) {
			// manage friends
			printf("Manage friends\n");
			//manage_friends(username);
		}
		else if (service_choice == 2) {
			// manage messages
			printf("Manage messages\n");
			//manage_messages(username);
		}
		else if (service_choice == 3)
			break;
		else
			printf("Invalid choice! Please enter a number between 1 and 3.\n");
	}
}
    
