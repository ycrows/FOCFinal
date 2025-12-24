#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 宏定义：消息最大数量/长度（移除未使用的"当前用户"宏）
#define MAX_MESSAGES 100  // 最多存储100条消息
#define MAX_LINE 200      // 每行内容最大长度

// 消息结构体（匹配思维导图的消息格式）
typedef struct {
    char from[50];    // 发送方
    char time[20];    // 消息时间（格式：2025-12-21 21:59:09）
    int unread;       // 未读标记（0=已读，1=未读）
    char content[200];// 消息内容
} Message;

/**
 * 从文件加载所有消息到结构体数组
 * @param messages 存储消息的数组
 * @param filename 消息文件路径（由用户名动态生成）
 * @return 实际加载的消息数量
 */
int load_messages(Message messages[], const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Message file %s does not exist or cannot be opened.\n", filename);
        return 0; // 文件不存在/打开失败，返回0条消息
    }

    char line[MAX_LINE];
    int msg_count = 0; // 已加载的消息数

    // 逐行解析消息（格式：FROM→TIME→UNREAD→CONTENT→---）
    while (msg_count < MAX_MESSAGES && fgets(line, MAX_LINE, fp) != NULL) {
        // 去除行尾换行符（避免解析错误）
        line[strcspn(line, "\n")] = '\0';
        
        // 解析FROM字段
        if (strstr(line, "FROM=") != NULL) {
            sscanf(line, "FROM=%s", messages[msg_count].from);
        }
        // 解析TIME字段（支持带空格的时间格式）
        else if (strstr(line, "TIME=") != NULL) {
            sscanf(line, "TIME=%[^\n]", messages[msg_count].time);
        }
        // 解析UNREAD字段
        else if (strstr(line, "UNREAD=") != NULL) {
            sscanf(line, "UNREAD=%d", &messages[msg_count].unread);
        }
        // 解析CONTENT字段（支持带空格的内容）
        else if (strstr(line, "CONTENT=") != NULL) {
            sscanf(line, "CONTENT=%[^\n]", messages[msg_count].content);
            // 跳过分隔符"---"行
            fgets(line, MAX_LINE, fp);
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
 * @param filename 消息文件路径
 */
void save_messages(Message messages[], int msg_count, const char *filename) {
    FILE *fp = fopen(filename, "w");
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
    printf("Messages saved successfully.\n");
}

/**
 * 打印所有消息（忽略未读状态）
 */
void read_all(Message messages[], int msg_count) {
    if (msg_count == 0) {
        printf("No messages found.\n");
        return;
    }

    printf("\n===== All Messages =====\n");
    // 遍历打印所有消息
    for (int i = 0; i < msg_count; i++) {
        printf("[%s] From: %s %s\n", messages[i].time, messages[i].from, 
               messages[i].unread ? "(Unread)" : "(Read)");
        printf("Content: %s\n\n", messages[i].content);
    }
}

/**
 * 打印未读消息并标记为已读（优化：无需传递msg_count指针，仅修改状态）
 */
void read_unread(Message messages[], int msg_count, const char *filename) {
    int has_unread = 0; // 标记是否有未读消息

    printf("\n===== Unread Messages =====\n");
    // 遍历打印未读消息，并修改为已读
    for (int i = 0; i < msg_count; i++) {
        if (messages[i].unread == 1) {
            has_unread = 1;
            printf("[%s] From: %s\n", messages[i].time, messages[i].from);
            printf("Content: %s\n\n", messages[i].content);
            messages[i].unread = 0; // 标记为已读
        }
    }

    if (!has_unread) {
        printf("No unread messages.\n\n");
    } else {
        save_messages(messages, msg_count, filename); // 保存修改后的状态
    }
}

/**
 * 消息读取主功能入口（动态接收用户名，无硬编码）
 * @param username 当前登录的用户名（用于生成消息文件：{username}.txt）
 */
void read_messages(const char *username) {
    if (username == NULL || strlen(username) == 0) {
        printf("Error: Invalid username!\n");
        return;
    }

    // 动态生成用户消息文件名（如：alice.txt）
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", username);

    Message messages[MAX_MESSAGES];
    int msg_count = load_messages(messages, filename);

    int choice;
    do {
        // 打印功能菜单
        printf("\n===== Message Menu for %s =====\n", username);
        printf("1. Read all messages\n");
        printf("2. Read unread messages only\n");
        printf("3. Back to main menu\n");
        printf("Enter your choice (1-3): ");
        
        // 处理非法输入，清理缓冲区
        while (scanf("%d", &choice) != 1) {
            getchar(); // 清空输入缓冲区的非法字符
            printf("Invalid input! Please enter a number (1-3): ");
        }
        getchar(); // 吸收换行符

        // 菜单功能分支
        switch (choice) {
            case 1:
                read_all(messages, msg_count);
                break; // 执行后回到菜单（原return改为break）
            case 2:
                read_unread(messages, msg_count, filename);
                break; // 执行后回到菜单（原return改为break）
            case 3:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice! Please enter 1-3.\n");
        }
    } while (choice != 3); // 选择3时退出循环
}

// 测试主函数（可根据实际业务逻辑替换）
int main() {
    // 示例：模拟传入用户名（实际使用时可从登录模块获取）
    char username[50];
    printf("Enter your username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // 去除换行符

    read_messages(username);
    return 0;
}
