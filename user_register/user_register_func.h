#ifndef USER_REGISTER
#define USER_REGISTER

// Hub function
void user_register();

// Database functions
void save_to_database(char username[], char password[]);

// Account management functions
int check_if_user_exists(char username[]);

#endif // USER_REGISTER

