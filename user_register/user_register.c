#include <stdio.h>
#include "user_register_func.h"
void user_register(){
    print("hello world!\n");
    // don't change the variables please
    char user[];
    char password[];
    char password_confirm[];

    // put code below to get the username

    // put code below to get the password

    // confirm the password / check the password
    
    // check if user in the database
    check_if_user_exists(char username[]); // return 0 if not in database, return 1 if in database
    
    // if user and password is correct --> put in database.txt
    save_to_database(char username[], char password[]);
    
}
