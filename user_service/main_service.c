#include <stdio.h>
void user_register(){
  




	int service_choice = 0;
  printf("==================== Main Service ====================\n");
  printf("1. Manage friends\n");
  printf("2. Manage messages\n");
	printf("3. Back\n");
	printf("===============================================\n");
	printf("Choose an option (1-3): ");
	
	while (service_choice != 1 && service_choice != 2 && service_choice != 3) {
		scanf("%d", &service_choice);
		if (service_choice == 1) {
			// manage friends
      manage_friends(char username[]);
			return 0;
		}
		else if (service_choice == 2) {
			// manage messages
      printf("Insert prototype later\n");
			return 0;
		}
		else if (service_choice == 3)
			// return to main.c
      else
			printf("Invalid choice! Please enter a number between 1 and 3.\n");
	}
}
    
