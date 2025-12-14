#include <extcmd/syshead.h>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    bool recursive = false;
    int opt;
    while ((opt = getopt(argc, argv, "r")) != -1)
    {
        switch (opt)
        {
        case 'r':
            recursive = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-r] file...\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (argc - optind < 1)
    {
        fprintf(stderr, "Usage: %s [-r] file...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = optind; i < argc; ++i)
    {
        fs::path path(argv[i]);
        try
        {
            if (recursive)
            {
                fs::remove_all(path);
            }
            else
            {
                fs::remove(path);
            }
        }
        catch (const fs::filesystem_error &e)
        {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
