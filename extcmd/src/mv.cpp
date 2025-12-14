#include <extcmd/syshead.h>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s source dest\n", argv[0]);
        return EXIT_FAILURE;
    }

    fs::path src(argv[1]);
    fs::path dst(argv[2]);

    try
    {
        fs::rename(src, dst);
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
