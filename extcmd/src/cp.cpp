#include <extcmd/syshead.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void my_copy_file(const fs::path &src, const fs::path &dst)
{
    std::ifstream src_file(src, std::ios::binary);
    std::ofstream dst_file(dst, std::ios::binary);
    dst_file << src_file.rdbuf();
}

void copy_recursive(const fs::path &src, const fs::path &dst)
{
    if (fs::is_directory(src))
    {
        fs::create_directories(dst);
        for (const auto &entry : fs::directory_iterator(src))
        {
            copy_recursive(entry.path(), dst / entry.path().filename());
        }
    }
    else
    {
        my_copy_file(src, dst);
    }
}

int main(int argc, char *argv[])
{
    bool recursive = false;
    int opt;
    while ((opt = getopt(argc, argv, "R")) != -1)
    {
        switch (opt)
        {
        case 'R':
            recursive = true;
            break;
        default:
            fprintf(stderr, "Usage: %s [-R] source dest\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (argc - optind != 2)
    {
        fprintf(stderr, "Usage: %s [-R] source dest\n", argv[0]);
        return EXIT_FAILURE;
    }

    fs::path src(argv[optind]);
    fs::path dst(argv[optind + 1]);

    try
    {
        if (recursive)
        {
            copy_recursive(src, dst);
        }
        else
        {
            if (fs::is_directory(dst))
            {
                dst /= src.filename();
            }
            fs::copy_file(src, dst);
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
