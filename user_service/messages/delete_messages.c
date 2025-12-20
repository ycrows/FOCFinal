#include <stdio.h>
#include <string.h>
#include <time.h>
//function prototype
void convert_date(char *input_date, char *output_date);
void get_msg_date(char *time_line, char *msg_date);
void get_msg_sender(char *from_line, char *sender);
int compare_date(char *date1, char *date2);
int has_messages(char *file_name);
//main function
void delete_messages(char username[]) {
    char file_name[50];
    sprintf(file_name, "%s.txt", username);

    if (!has_messages(file_name)) {
        printf("No messages found\n");
        return;
    }

    char start_input[11], end_input[11], target_sender[20];
    char start_date[11], end_date[11];
    printf("Enter start date (dd/mm/yyyy): ");
    scanf("%s", start_input);
    printf("Enter end date (dd/mm/yyyy): ");
    scanf("%s", end_input);
    printf("Enter account name (or 'all'): ");
    scanf("%s", target_sender);

    convert_date(start_input, start_date);
    convert_date(end_input, end_date);

    FILE *fp_read = fopen(file_name, "r");
    char keep_msgs[200][800];
    int keep_count = 0, delete_count = 0;
    char line[200], msg_block[800];
    char msg_date[11], msg_sender[20], from_line[200], time_line[200];
    memset(msg_block, 0, sizeof(msg_block));
    int has_from = 0, has_time = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        strcat(msg_block, line);
        if (strstr(line, "FROM=")) { strcpy(from_line, line); has_from = 1; }
        if (strstr(line, "TIME=")) { strcpy(time_line, line); has_time = 1; }

        if (strcmp(line, "---\n") == 0) {
            int need_delete = 0;
            if (has_from && has_time) {
                get_msg_sender(from_line, msg_sender);
                get_msg_date(time_line, msg_date);
                if (compare_date(start_date, msg_date) && compare_date(msg_date, end_date)) {
                    if (strcmp(target_sender, "all") == 0 || strcmp(target_sender, msg_sender) == 0) {
                        need_delete = 1;
                        delete_count++;
                    }
                }
            }
            if (!need_delete) strcpy(keep_msgs[keep_count++], msg_block);
            memset(msg_block, 0, sizeof(msg_block));
            has_from = has_time = 0;
        }
    }
    fclose(fp_read);
    FILE *fp_write = fopen(file_name, "w");
    for (int i = 0; i < keep_count; i++) fputs(keep_msgs[i], fp_write);
    fclose(fp_write);
    printf("Removed %d message(s) from %s in period %s - %s.\n", delete_count, target_sender, start_input, end_input);
}
//date form transformation
void convert_date(char *input_date, char *output_date) {
    char day[3], month[3], year[5];
    strncpy(day, input_date, 2);
    strncpy(month, input_date + 3, 2);
    strncpy(year, input_date + 6, 4);
    day[2] = month[2] = year[4] = '\0';
    sprintf(output_date, "%s-%s-%s", year, month, day);
}
//date reading
void get_msg_date(char *time_line, char *msg_date) {
    strncpy(msg_date, time_line + 5, 10);
    msg_date[10] = '\0';
}
//sender reading
void get_msg_sender(char *from_line, char *sender) {
    int i = 5, j = 0;
    while (from_line[i] != '\n') sender[j++] = from_line[i++];
    sender[j] = '\0';
}
//date comparing
int compare_date(char *date1, char *date2) {
    struct tm tm1 = {0}, tm2 = {0};
    tm1.tm_year = atoi(date1) - 1900;
    tm1.tm_mon = atoi(date1 + 5) - 1;
    tm1.tm_mday = atoi(date1 + 8);
    tm2.tm_year = atoi(date2) - 1900;
    tm2.tm_mon = atoi(date2 + 5) - 1;
    tm2.tm_mday = atoi(date2 + 8);
    return mktime(&tm1) <= mktime(&tm2);
}
//message nonempty justice
int has_messages(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) return 0;
    char line[200];
    int has_content = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) > 1) {
            has_content = 1;
            break;
        }
    }
    fclose(fp);
    return has_content;
}
