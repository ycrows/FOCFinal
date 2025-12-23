#ifndef SERVICE_FRIENDS
#define SERVICE_FRIENDS

void accept_friends(char username[]);
void add_friends(char sender_name[]);
void delete_friends(char username[]);
void manage_friends(char username[]);
void show_friends(char username[]);
char *my_strtok_r(char *str, const char *delim, char **saveptr);
void request_builder(FILE *fp, char username[], char request_list[][50], int *request_count, int *user_found, int mode);


#endif 
