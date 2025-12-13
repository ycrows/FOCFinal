#ifndef USER_REGISTER
#define USER_REGISTER

// Database functions
void save_to_database(char username[], char password[]);

// Account management functions
int check_if_user_exists(char username[]);

#endif // USER_REGISTER