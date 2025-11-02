#include <shell/shellhead.h>

void setup_readline()
{
    rl_variable_bind("horizontal-scroll-mode", "1");
    rl_variable_bind("colored-completion-prefix", "1");
}