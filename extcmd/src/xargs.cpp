#include <extcmd/syshead.h>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s utility [argument ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::vector<std::string> initial_args;
    for (int i = 1; i < argc; ++i)
    {
        initial_args.push_back(argv[i]);
    }

    std::vector<std::string> input_args;
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), stdin))
    {
        char *token = strtok(buffer, " \t\n");
        while (token)
        {
            input_args.push_back(token);
            token = strtok(NULL, " \t\n");
        }
    }

    // For simplicity, run once
    std::vector<char *> exec_args;
    for (auto &arg : initial_args)
    {
        exec_args.push_back(const_cast<char *>(arg.c_str()));
    }
    for (auto &arg : input_args)
    {
        exec_args.push_back(const_cast<char *>(arg.c_str()));
    }
    exec_args.push_back(NULL);

    execvp(initial_args[0].c_str(), exec_args.data());
    perror("execvp");
    return EXIT_FAILURE;
}
