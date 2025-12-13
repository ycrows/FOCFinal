#include <stdio.h>
#include "user_register/register.h"

int main(int argc, char *argv[])
{
	int login_choice = 0;
    printf("==================== Login ====================\n");
    printf("1. Login\n");
    printf("2. Register (If you do not have an account)\n");
	printf("3. Exit\n");
	printf("===============================================\n");
	printf("Choose an option (1-3): ");
	
	while (login_choice != 1 && login_choice != 2 && login_choice != 3) {
		scanf("%d", &login_choice);
		if (login_choice == 1) {
			char* username[50];
			char* password[50];
			printf("Please input your account name: ");
			scanf("%s", username);
			printf("and password: ");
			scanf("%s", password);
						
			// check if account exists
			if ("declare condition") {
				// if account exists check if password is correct
				if ("declare condition") {
					user_service();
				}
			}
			return 0;
		}
		else if (login_choice == 2) {
			user_register();
			return 0;
		}
		else if (login_choice == 3)
			return 0;
	    else
			printf("Invalid choice! Please enter a number between 1 and 3.\n");
	}
}
    
