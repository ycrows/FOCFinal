#include <stdio.h>
#include <string.h>   
#include "user_register_func.h"

void user_register(){

    // don't change the variables please
    char user[50];
    char password[50];
    char password_confirm[50];

    // put code below to get the username
    printf("Enter username: ");
    scanf("%s", user);

    // put code below to get the password
    printf("Enter password: ");
    scanf("%s", password);

    // confirm the password / check the password
    printf("Confirm password: ");
    scanf("%s", password_confirm);

    if(strcmp(password, password_confirm) != 0){
        printf("Passwords do not match. Registration failed.\n");
        return; // 直接退出函数，不继续执行 exit function
    }

    // check if user in the database
    if(check_if_user_exists(user) == 1){
        printf("Error: User already exists!\n");
        return;
    }

    // if user and password is correct --> put in database.txt
    save_to_database(user, password);
    printf("Registration successful! You can now login.\n");
}

