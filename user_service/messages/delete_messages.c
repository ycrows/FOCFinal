#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void delete_messages(char username[]);
static void convert_date(char *input_date, char *output_date);
static void get_msg_date(char *time_line, char *msg_date);
static void get_msg_sender(char *from_line, char *sender);
static int compare_date(char *date1, char *date2);
static int has_messages(char *file_name);
static int is_valid_date_format(char *date);

static int is_valid_date_format(char *date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '/' || date[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return 0;
    }
    return 1;
}

static void convert_date(char *input_date, char *output_date) {
    char day[3], month[3], year[5];
    strncpy(day, input_date, 2);
    day[2] = '\0';
    strncpy(month, input_date + 3, 2);
    month[2] = '\0';
    strncpy(year, input_date + 6, 4);
    year[4] = '\0';
    snprintf(output_date, 11, "%s-%s-%s", year, month, day);
}

static void get_msg_date(char *time_line, char *msg_date) {
    strncpy(msg_date, time_line + 5, 10);
    msg_date[10] = '\0';
}

static void get_msg_sender(char *from_line, char *sender) {
    int i = 5, j = 0;
    while (from_line[i] != '\n' && from_line[i] != '\r' && from_line[i] != '\0' && j < 19) {
        sender[j++] = from_line[i++];
    }
    sender[j] = '\0';
}

static int compare_date(char *date1, char *date2) {
    struct tm tm1 = {0}, tm2 = {0};
    char year1[5], month1[3], day1[3];
    char year2[5], month2[3], day2[3];

    strncpy(year1, date1, 4); year1[4] = '\0';
    strncpy(month1, date1 + 5, 2); month1[2] = '\0';
    strncpy(day1, date1 + 8, 2); day1[2] = '\0';
    strncpy(year2, date2, 4); year2[4] = '\0';
    strncpy(month2, date2 + 5, 2); month2[2] = '\0';
    strncpy(day2, date2 + 8, 2); day2[2] = '\0';

    tm1.tm_year = atoi(year1) - 1900;
    tm1.tm_mon = atoi(month1) - 1;
    tm1.tm_mday = atoi(day1);
    tm2.tm_year = atoi(year2) - 1900;
    tm2.tm_mon = atoi(month2) - 1;
    tm2.tm_mday = atoi(day2);

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    if (t1 == -1 || t2 == -1) return 0;
    return t1 <= t2;
}

static int has_messages(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) return 0;

    char line[200];
    int has_content = 0;
    while (fgets(line, sizeof(line), fp)) {
        int is_blank = 1;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r') {
                is_blank = 0;
                break;
            }
        }
        if (!is_blank) {
            has_content = 1;
            break;
        }
    }
    fclose(fp);
    return has_content;
}

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
    if (!is_valid_date_format(start_input)) {
        printf("Invalid start date format! Must be dd/mm/yyyy.\n");
        return;
    }

    printf("Enter end date (dd/mm/yyyy): ");
    scanf("%s", end_input);
    if (!is_valid_date_format(end_input)) {
        printf("Invalid end date format! Must be dd/mm/yyyy.\n");
        return;
    }

    printf("Enter account name (or 'all'): ");
    scanf("%s", target_sender);

    convert_date(start_input, start_date);
    convert_date(end_input, end_date);

    if (!compare_date(start_date, end_date)) {
        printf("Start date cannot be later than end date!\n");
        return;
    }

    FILE *fp_read = fopen(file_name, "r");
    if (fp_read == NULL) {
        printf("Failed to open file %s for reading.\n", file_name);
        return;
    }

    char keep_msgs[200][800];
    int keep_count = 0, delete_count = 0;
    char line[200], msg_block[800];
    char msg_date[11], msg_sender[20], from_line[200], time_line[200];
    memset(msg_block, 0, sizeof(msg_block));
    int has_from = 0, has_time = 0;

    while (fgets(line, sizeof(line), fp_read)) {
        if (strlen(msg_block) + strlen(line) < sizeof(msg_block) - 1) {
            strcat(msg_block, line);
        }

        if (strstr(line, "FROM=")) {
            strncpy(from_line, line, sizeof(from_line) - 1);
            from_line[sizeof(from_line)-1] = '\0';
            has_from = 1;
        }

        if (strstr(line, "TIME=")) {
            strncpy(time_line, line, sizeof(time_line) - 1);
            time_line[sizeof(time_line)-1] = '\0';
            has_time = 1;
        }

        if (strstr(line, "---") != NULL) {
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

            if (!need_delete && keep_count < 200) {
                strncpy(keep_msgs[keep_count++], msg_block, sizeof(keep_msgs[0]) - 1);
                keep_msgs[keep_count-1][sizeof(keep_msgs[0])-1] = '\0';
            }

            memset(msg_block, 0, sizeof(msg_block));
            has_from = has_time = 0;
        }
    }

    fclose(fp_read);

    if (keep_count == 0 && delete_count == 0) {
        printf("No messages matched the delete criteria, file remains unchanged.\n");
        return;
    }

    FILE *fp_write = fopen(file_name, "w");
    if (fp_write == NULL) {
        printf("Failed to open file %s for writing.\n", file_name);
        return;
    }

    for (int i = 0; i < keep_count; i++) {
        fputs(keep_msgs[i], fp_write);
    }
    fclose(fp_write);

    if (delete_count == 0) {
        printf("No messages matched the criteria (sender: %s, period %s - %s), file not modified.\n", target_sender, start_input, end_input);
    } else {
        printf("Removed %d message(s) from %s in period %s - %s.\n", delete_count, target_sender, start_input, end_input);
    }
}
