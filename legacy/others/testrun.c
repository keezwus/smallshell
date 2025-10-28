#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main()
{

    struct termios origTerm, rawTerm;
    if (tcgetattr(STDIN_FILENO, &origTerm) != 0)
    {
        perror("tcGetAttrErr");
    }
    rawTerm = origTerm;
    rawTerm.c_lflag &= ~(ECHO | ICANON);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTerm) != 0)
    {
        perror("tcSetAttrErr");
    }
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        printf("Received: %d (0x%02x)", (int)c, (unsigned char)c);
        if (c >= 32 && c < 127)
            printf(" -> '%c'", c);
        printf("\n");

        if (c == 'q')
            break; // 按 q 退出
    }
    return 0;
}
