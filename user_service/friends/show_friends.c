#include <stdio.h>
#include <string.h>

void show_friends(char username[]){
    
    #define MAX_LINE_LENGTH 256
    #define MAX_FRIENDS 10
    #define MAX_NAME_LENGTH 50

    FILE *fp = fopen("database.txt", "r");
    char line[MAX_LINE_LENGTH];
    char friends[MAX_FRIENDS][MAX_NAME_LENGTH];
    int friend_count = 0;
    int user_found = 0;

    // Check if database exists
    if (fp == NULL) {
        return;
    }

    // Find the logged-in user's data and extract friends
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        // Match "name=" line to find the logged-in user
        if (strncmp(line, "name=", 5) == 0) {
            char other_user[MAX_NAME_LENGTH];
            strncpy(other_user, line + 5, sizeof(other_user) - 1);
            other_user[strcspn(other_user, "\n")] = '\0'; // Remove newline

            // If this is the logged-in user, get their friends
            if (strcmp(other_user, username) == 0) {
                user_found = 1;

                // Skip lines until we hit "friends="
                while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
                    if (strncmp(line, "friends=", 8) == 0) {
                        char *current = line + 8; // Skip "friends=" prefix

                        // Parse comma-separated friends
                        while (*current != '\0' && *current != '\n' && friend_count < MAX_FRIENDS) {
                            while (*current == ' ') current++; // Skip spaces
                            char *name_start = current;
                            while (*current != ',' && *current != '\n' && *current != '\0') current++; // Find end of name
                            int name_len = current - name_start;

                            // Add friend to list if valid
                            if (name_len > 0) {
                                strncpy(friends[friend_count], name_start, name_len);
                                friends[friend_count][name_len] = '\0';
                                friend_count++;
                            }

                            // Skip comma for next friend
                            if (*current == ',') {
                                current++;
                                while (*current == ' ') current++;
                            }
                        }
                        break; // Stop after finding friends
                    }
                }
                break; // Stop after finding the user
            }
        }
    }

    // Display friends (clean, user-friendly output)
    if (friend_count == 0) {
        printf("You have no friends.\n");
    } else {
        printf("Your friends:\n");
        for (int i = 0; i < friend_count; i++) {
            printf("%d. %s\n", i + 1, friends[i]);
        }
    }
    fclose(fp);
}

/*
int main() {
    // --- THIS PART IS JUST AN EXAMPLE OF YOUR EXISTING LOGIN FLOW ---
    // Assume the user has already logged in, so we have their other_user:
    const char *logged_in_user = "walter"; // Replace with your actual logged-in other_user variable
    printf("Welcome back, %s!\n", logged_in_user);
    printf("... (your menu here: 1=Add, 2=Accept, 3=Delete, 4=Show Friends, 5=Back)\n");
    printf("You pressed 4 to show friends.\n");

    // --- THIS IS WHERE YOU CALL THE FUNCTION WHEN THEY PRESS 4 ---
    showCurrentFriends(logged_in_user);

    return 0;
}







}
*/