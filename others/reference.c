#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_HISTORY 10   // 最大历史记录数
#define BUFFER_SIZE 1024 // 缓冲区大小

// 定义历史记录结构体，用于存储历史命令
typedef struct
{
    char *commands[MAX_HISTORY]; // 指向命令字符串的指针数组
    int count;                   // 当前存储的命令数量
    int current;                 // 当前历史位置（用于浏览历史记录）
} History;

// 初始化历史记录结构体
void init_history(History *history)
{
    history->count = 0;    // 初始化命令计数
    history->current = -1; // 当前历史位置置为 -1
    for (int i = 0; i < MAX_HISTORY; i++)
    {
        history->commands[i] = NULL; // 初始化指针数组为空
    }
}

// 添加新命令到历史记录
void add_history(History *history, const char *command)
{
    if (history->count < MAX_HISTORY)
    {                                                          // 如果历史记录未满
        history->commands[history->count++] = strdup(command); // 复制命令并添加
    }
    else
    {                               // 如果历史记录已满
        free(history->commands[0]); // 释放最早的命令内存
        for (int i = 1; i < MAX_HISTORY; i++)
        {
            history->commands[i - 1] = history->commands[i]; // 依次前移命令
        }
        history->commands[MAX_HISTORY - 1] = strdup(command); // 添加新命令到最后
    }
    history->current = history->count; // 重置当前历史位置
}

// 获取历史记录中指定方向的命令
const char *get_history(History *history, int direction)
{
    if (direction == -1 && history->current > 0)
    { // 上箭头（浏览旧命令）
        return history->commands[--history->current];
    }
    else if (direction == 1 && history->current < history->count - 1)
    { // 下箭头（浏览新命令）
        return history->commands[++history->current];
    }
    return NULL; // 超出历史记录范围时返回 NULL
}

// 设置终端为原始模式（关闭行缓冲和回显）
void set_raw_mode(struct termios *orig_termios)
{
    struct termios raw = *orig_termios;       // 复制当前终端设置
    raw.c_lflag &= ~(ECHO | ICANON);          // 关闭回显和行缓冲
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // 应用新的终端设置
}

// 恢复终端为原始模式前的设置
void restore_mode(struct termios *orig_termios)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios); // 恢复终端设置
}

// 实现类似 readline 的功能，支持历史记录浏览
char *readline(const char *prompt, History *history)
{
    struct termios orig_termios;
    tcgetattr(STDIN_FILENO, &orig_termios); // 获取终端的当前设置
    set_raw_mode(&orig_termios);            // 设置为原始模式

    if (prompt)
    {
        printf("%s", prompt); // 输出提示符
        fflush(stdout);       // 刷新输出
    }

    char buffer[BUFFER_SIZE];       // 存储输入的缓冲区
    int pos = 0;                    // 当前缓冲区位置
    const char *history_cmd = NULL; // 指向历史命令

    while (1)
    {
        char c;
        if (read(STDIN_FILENO, &c, 1) != 1)
        {                                // 逐字符读取用户输入
            restore_mode(&orig_termios); // 出错时恢复终端设置
            return NULL;
        }

        if (c == '\n')
        {                                // 处理 Enter 键
            buffer[pos] = '\0';          // 结束缓冲区字符串
            printf("\n");                // 换行显示
            restore_mode(&orig_termios); // 恢复终端设置
            return strdup(buffer);       // 返回输入的完整字符串
        }
        else if (c == 127 || c == 8)
        { // 处理退格键
            if (pos > 0)
            {
                pos--;           // 缓冲区位置后退
                printf("\b \b"); // 删除字符
                fflush(STDIN_FILENO);
            }
        }
        else if (c == 27)
        { // 处理箭头键序列
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1)
                continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1)
                continue;
            if (seq[0] == '[')
            {
                if (seq[1] == 'A')
                { // 上箭头，获取上一条命令
                    history_cmd = get_history(history, -1);
                }
                else if (seq[1] == 'B')
                { // 下箭头，获取下一条命令
                    history_cmd = get_history(history, 1);
                }
                if (history_cmd)
                { // 有历史命令可用时，显示历史命令
                    while (pos > 0)
                    { // 清空当前行
                        printf("\b \b");
                        pos--;
                    }
                    strncpy(buffer, history_cmd, BUFFER_SIZE); // 将历史命令复制到缓冲区
                    pos = strlen(buffer);                      // 更新缓冲区位置
                    printf("%s", buffer);                      // 显示历史命令
                    fflush(stdout);
                }
            }
        }
        else
        { // 普通字符
            if (pos < BUFFER_SIZE - 1)
            {
                buffer[pos++] = c; // 将字符存入缓冲区
                printf("%c", c);   // 显示字符
                fflush(stdout);
            }
        }
    }
}

int main()
{
    History history;
    init_history(&history); // 初始化历史记录

    while (1)
    {
        char *input = readline("请输入命令: ", &history); // 调用 readline
        if (input == NULL)
        {
            break; // 若输入为空，则退出循环
        }
        if (strcmp(input, "exit") == 0)
        { // 输入 "exit" 则退出程序
            free(input);
            break;
        }
        printf("您输入的命令是: %s\n", input); // 输出用户输入的命令
        add_history(&history, input);          // 添加输入到历史记录
        free(input);                           // 释放输入字符串的内存
    }

    // 释放历史记录中的内存
    for (int i = 0; i < history.count; i++)
    {
        free(history.commands[i]);
    }

    return 0;
}