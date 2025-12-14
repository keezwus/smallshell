#include <extcmd/syshead.h>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    bool symbolic = false;
    int opt;
    while ((opt = getopt(argc, argv, "s")) != -1)
    {
        switch (opt)
        {
        case 's':
            symbolic = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-s] target link_name\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (argc - optind != 2)
    {
        fprintf(stderr, "Usage: %s [-s] target link_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    fs::path target(argv[optind]);
    fs::path link_name(argv[optind + 1]);

    try
    {
        if (symbolic)
        {
            fs::create_symlink(target, link_name);
        }
        else
        {
            fs::create_hard_link(target, link_name);
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
