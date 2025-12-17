#include <stdio.h>
#include <string.h>
#include <time.h>

// --------------------------
// Constant Definitions
// --------------------------
#define MAX_LINE_LENGTH 256
#define MAX_FRIENDS 10
#define MAX_NAME_LENGTH 50
#define MAX_FIELD_LENGTH 256
#define MESSAGE_FILE "messages.txt"
#define DATABASE_FILE "database.txt"
#define MAX_SELECTIONS 10 // Max number of friends to select at once

// --------------------------
// Data Structure
// --------------------------
typedef struct {
    char to[MAX_FIELD_LENGTH];
    char from[MAX_FIELD_LENGTH];
    char time[MAX_FIELD_LENGTH];
    int unread;
    char content[MAX_FIELD_LENGTH];
} Message;

// --------------------------
// Helper Function: Custom Atoi
// --------------------------
int atoi_custom(const char *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            res = res * 10 + (str[i] - '0');
        } else {
            break; // Stop at non-digit
        }
    }
    return res;
}

// --------------------------
// Helper Function: Check if file is empty
// --------------------------
int is_file_empty(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return 1; // If file doesn't exist, treat it as empty
    }
    int is_empty = (fgetc(fp) == EOF) ? 1 : 0;
    fclose(fp);
    return is_empty;
}

// --------------------------
// Helper Function: Check if input contains any commas
// --------------------------
int has_comma(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
            return 1; // Comma found
        }
    }
    return 0; // No comma
}

// --------------------------
// Function: Show Friends (with All + Back)
// --------------------------
int showCurrentFriends(const char *logged_in_username, char friends[MAX_FRIENDS][MAX_NAME_LENGTH]) {
    FILE *fp = fopen(DATABASE_FILE, "r");
    char line[MAX_LINE_LENGTH];
    int friend_count = 0;
    int user_found = 0;

    if (fp == NULL) {
        printf("Error: Could not open database file.\n");
        return 0;
    }
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        if (strncmp(line, "name=", 5) == 0) {
            char username[MAX_NAME_LENGTH];
            strncpy(username, line + 5, sizeof(username) - 1);
            username[strcspn(username, "\n")] = '\0';

            if (strcmp(username, logged_in_username) == 0) {
                user_found = 1;
                while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
                    if (strncmp(line, "friends=", 8) == 0) {
                        char *current = line + 8;
                        while (*current != '\0' && *current != '\n' && friend_count < MAX_FRIENDS) {
                            while (*current == ' ') current++;
                            char *name_start = current;
                            while (*current != ',' && *current != '\n' && *current != '\0') current++;
                            int name_len = current - name_start;

                            if (name_len > 0) {
                                strncpy(friends[friend_count], name_start, name_len);
                                friends[friend_count][name_len] = '\0';
                                friend_count++;
                            }

                            if (*current == ',') {
                                current++;
                                while (*current == ' ') current++;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
    fclose(fp);

    printf("Your friends:\n");
    if (friend_count == 0) {
        printf("You have no friends yet.\n");
    } else {
        for (int i = 0; i < friend_count; i++) {
            printf("%d. %s\n", i + 1, friends[i]);
        }
        printf("%d. All\n", friend_count + 1);
    }
    int back_option = (friend_count > 0) ? (friend_count + 2) : 1;
    printf("%d. Back\n", back_option);
    return friend_count;
}

// --------------------------
// Function: Store Message to File (NO leading newline)
// --------------------------
int store_message(const Message *message) {
    FILE *fp = fopen(MESSAGE_FILE, "a");
    if (fp == NULL) {
        perror("Error opening messages file");
        return -1;
    }

    int file_empty = is_file_empty(MESSAGE_FILE);
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    if (!file_empty) {
        fprintf(fp, "\n");
    }

    fprintf(fp, "TO=%s\n", message->to);
    fprintf(fp, "FROM=%s\n", message->from);
    fprintf(fp, "TIME=%s\n", timestamp);
    fprintf(fp, "UNREAD=1\n");
    fprintf(fp, "CONTENT=%s\n", message->content);
    fprintf(fp, "---");

    fclose(fp);
    return 0;
}

// --------------------------
// Function: Send Message (message FIRST, then friends/selection)
// --------------------------
int send_messages(const char *sender) {
    // --------------------------
    // STEP 1: Enter message FIRST
    // --------------------------
    char content[MAX_FIELD_LENGTH];
    printf("Enter message (max 255 chars), press Enter to finish: ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = '\0'; // Remove newline

    if (strlen(content) == 0) {
        printf("Error: Message cannot be empty!\n");
        return -1;
    }

    // --------------------------
    // STEP 2: Show friends list
    // --------------------------
    char friends[MAX_FRIENDS][MAX_NAME_LENGTH];
    int friend_count = showCurrentFriends(sender, friends);
    int all_option = friend_count + 1;
    int back_option = (friend_count > 0) ? (friend_count + 2) : 1;

    if (friend_count == 0) {
        printf("Cannot send message: No friends to send to.\n");
        return -1;
    }

    // --------------------------
    // STEP 3: Pick recipient option(s) - SPACE-ONLY
    // --------------------------
    char input[256];
    printf("Enter friend numbers (seperated by space), press Enter to finish: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    // Check for Back option (exact input only)
    if (strcmp(input, " ") != 0 && atoi_custom(input) == back_option && strlen(input) == 1) {
        printf("Returning to previous menu...\n");
        return 0;
    }

    // --------------------------
    // KEY: Check for commas + show warning
    // --------------------------
    if (has_comma(input)) {
        printf("WARNING: Commas are not allowed! Please use spaces only to separate selections.\n");
        printf("Aborting message send.\n");
        return -1;
    }

    Message new_msg;
    strcpy(new_msg.from, sender);
    strcpy(new_msg.content, content);
    int send_success = 1;

    // Check for All option (exact input only)
    if (strcmp(input, " ") != 0 && atoi_custom(input) == all_option && strlen(input) == 1) {
        // Send to all
        for (int i = 0; i < friend_count; i++) {
            strcpy(new_msg.to, friends[i]);
            if (store_message(&new_msg) != 0) {
                send_success = 0;
            }
        }
        // Feedback
        if (send_success) {
            printf("Message sent to All\n");
        } else {
            printf("Message sent to most friends, but some failures occurred.\n");
        }
    }
    // Case 2: Selected multiple/single friends (space-separated)
    else {
        char *token = strtok(input, " "); // Split ONLY by spaces (no commas)
        int selections[MAX_SELECTIONS];
        int sel_count = 0;

        // Parse all selections
        while (token != NULL && sel_count < MAX_SELECTIONS) {
            selections[sel_count] = atoi_custom(token);
            sel_count++;
            token = strtok(NULL, " ");
        }

        // Validate selections
        int valid = 1;
        for (int i = 0; i < sel_count; i++) {
            if (selections[i] < 1 || selections[i] > friend_count) {
                printf("Invalid option: %d\n", selections[i]);
                valid = 0;
            }
        }
        if (!valid) {
            printf("Aborting message send.\n");
            return -1;
        }

        // Send to selected friends + correct name output
        for (int i = 0; i < sel_count; i++) {
            int friend_idx = selections[i] - 1;
            strcpy(new_msg.to, friends[friend_idx]);
            if (store_message(&new_msg) == 0) {
                printf("Message sent to %s\n", friends[friend_idx]);
            } else {
                printf("Failed to send to %s\n", friends[friend_idx]);
                send_success = 0;
            }
        }
     }

    return 0;
}

/*
// --------------------------
// Main Function (Example)
// --------------------------
int main() {
    char username[MAX_NAME_LENGTH];
    printf("Enter your username (to simulate login): ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; "HAVE TO INCLUDE THIS"
    send_messages(username);
    return 0;
}
