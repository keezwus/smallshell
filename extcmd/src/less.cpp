#include <extcmd/syshead.h>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " file" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        perror(argv[1]);
        return EXIT_FAILURE;
    }

    std::string line;
    int line_count = 0;
    const int lines_per_page = 24; // approximate
    char ch;

    while (std::getline(file, line) && ch != 'q')
    {
        std::cout << line << std::endl;
        line_count++;
        if (line_count % lines_per_page == 0)
        {
            std::cout << "--More-- (press enter)" << std::endl;
            std::cin.get(ch);
        }
    }

    return EXIT_SUCCESS;
}
