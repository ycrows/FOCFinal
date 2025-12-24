#include <stdio.h>
#include "user_register/user_register_func.h"
#include "login_check.h"
#include "user_service/main_service.h"

int main()
{
	while (1) {
		int login_choice = 0;
		char input[100]; 
		printf("==================== Login ====================\n");
		printf("1. Login\n");
		printf("2. Register (If you do not have an account)\n");
		printf("3. Exit\n");
		printf("===============================================\n");
		printf("Choose an option (1-3): ");
		
		if (fgets(input, sizeof(input), stdin) != NULL) {
			if (sscanf(input, "%d", &login_choice) == 1) {
				// if login choice is a number
				if (login_choice == 1) {
					char username[50];
					char password[50];
					printf("Please input your account name: ");
					scanf("%s", username);
					printf("and password: ");
					scanf("%s", password);
					int c;
					while ((c = getchar()) != '\n' && c != EOF); // consume new lines
								
					// use login checker
					if (login_checker(username, password) == 1) {
						main_service(username);
						continue; 
					}
				}
				else if (login_choice == 2) {
					user_register();
				}
				else if (login_choice == 3) {
					printf("Byebye!\n");
					return 0;
				}
				else
					printf("Invalid choice! Please enter a number between 1 and 3.\n");
			}
			else {
				printf("Warning! Invalid input.\n");
				continue;
			}
		} 
	}	
}
    
