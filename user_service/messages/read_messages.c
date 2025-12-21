//
//  main.c
//  group project
//
//  Created by wang on 12/9/25.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 宏定义：当前用户、消息文件、消息最大数量/长度
#define CURRENT_USER "charles"
#define MESSAGE_FILE CURRENT_USER ".txt"
#define MAX_MESSAGES 100  // 最多存储100条消息
#define MAX_LINE 200      // 每行内容最大长度

// 消息结构体（匹配思维导图的消息格式）
typedef struct {
    char from[50];    // FROM字段
    char time[20];    // TIME字段（格式：2025-12-21 21:59:09）
    int unread;       // UNREAD字段（0=已读，1=未读）
    char content[200];// CONTENT字段
} Message;


/**
 * 从文件加载所有消息到结构体数组
 * @param messages 存储消息的数组
 * @return 消息的实际数量
 */
int load_messages(Message messages[]) {
    FILE *fp = fopen(MESSAGE_FILE, "r");
    if (fp == NULL) {
        return 0; // 文件不存在/打开失败，返回0条消息
    }

    char line[MAX_LINE];
    int msg_count = 0; // 已加载的消息数

    // 逐行解析消息（格式：FROM→TIME→UNREAD→CONTENT→---）
    while (msg_count < MAX_MESSAGES && fgets(line, MAX_LINE, fp) != NULL) {
        // 解析FROM字段
        if (strstr(line, "FROM=") != NULL) {
            sscanf(line, "FROM=%s", messages[msg_count].from);
        }
        // 解析TIME字段（读取整行，包含空格）
        else if (strstr(line, "TIME=") != NULL) {
            sscanf(line, "TIME=%[^\n]", messages[msg_count].time);
        }
        // 解析UNREAD字段
        else if (strstr(line, "UNREAD=") != NULL) {
            sscanf(line, "UNREAD=%d", &messages[msg_count].unread);
        }
        // 解析CONTENT字段（读取整行，包含空格）
        else if (strstr(line, "CONTENT=") != NULL) {
            sscanf(line, "CONTENT=%[^\n]", messages[msg_count].content);
            fgets(line, MAX_LINE, fp); // 跳过分隔符"---"
            msg_count++; // 完成一条消息的解析
        }
    }

    fclose(fp);
    return msg_count;
}


/**
 * 将修改后的消息数组写回文件
 * @param messages 消息数组
 * @param msg_count 消息数量
 */
void save_messages(Message messages[], int msg_count) {
    FILE *fp = fopen(MESSAGE_FILE, "w");
    if (fp == NULL) {
        perror("Failed to save messages");
        return;
    }

    // 按格式写入每条消息
    for (int i = 0; i < msg_count; i++) {
        fprintf(fp, "FROM=%s\n", messages[i].from);
        fprintf(fp, "TIME=%s\n", messages[i].time);
        fprintf(fp, "UNREAD=%d\n", messages[i].unread);
        fprintf(fp, "CONTENT=%s\n", messages[i].content);
        if (i != msg_count - 1) { // 最后一条消息后不写分隔符
            fprintf(fp, "---\n");
        }
    }

    fclose(fp);
}


/**
 * 实现“read all”功能：打印所有消息（忽略UNREAD标签）
 */
void read_all(Message messages[], int msg_count) {
    printf("\n===== All Messages (TO: %s) =====\n", CURRENT_USER);
    if (msg_count == 0) {
        printf("No messages found.\n");
        return;
    }

    // 遍历打印所有消息
    for (int i = 0; i < msg_count; i++) {
        printf("From: %s\n", messages[i].from);
        printf("Time: %s\n", messages[i].time);
        printf("Content: %s\n", messages[i].content);
        printf("------------------------\n");
    }
}


/**
 * 实现“read unread”功能：打印未读消息，并标记为已读
 */
void read_unread(Message messages[], int *msg_count) {
    printf("\n===== Unread Messages (TO: %s) =====\n", CURRENT_USER);
    int has_unread = 0; // 标记是否有未读消息

    // 遍历打印未读消息，并修改UNREAD为0
    for (int i = 0; i < *msg_count; i++) {
        if (messages[i].unread == 1) {
            has_unread = 1;
            printf("From: %s\n", messages[i].from);
            printf("Time: %s\n", messages[i].time);
            printf("Content: %s\n", messages[i].content);
            printf("------------------------\n");
            messages[i].unread = 0; // 标记为已读
        }
    }

    if (!has_unread) {
        printf("No unread messages.\n");
    } else {
        save_messages(messages, *msg_count); // 保存修改后的消息
        printf("All unread messages marked as read.\n");
    }
}


/**
 * read messages的交互式菜单
 */
void read_messages_menu() {
    Message messages[MAX_MESSAGES];
    int msg_count = load_messages(messages); // 加载消息
    int choice;

    do {
        // 打印菜单
        printf("\n===== Read Messages Menu =====\n");
        printf("1. Read All Messages\n");
        printf("2. Read Unread Messages\n");
        printf("3. Back\n");
        printf("Enter your choice (1-3): ");
        scanf("%d", &choice);
        getchar(); // 吸收输入后的换行符

        // 处理用户选择
        switch (choice) {
            case 1:
                read_all(messages, msg_count);
                break;
            case 2:
                read_unread(messages, &msg_count);
                break;
            case 3:
                printf("Returning to Manage Messages Menu...\n");
                break;
            default:
                printf("Invalid choice! Please enter 1-3.\n");
        }
    } while (choice != 3); // 选择3则退出菜单
}


// 主函数（入口）～_～
int main() {
    read_messages_menu();
    return 0;
}
