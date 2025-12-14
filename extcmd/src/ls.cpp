#include <extcmd/syshead.h>

int main(int argc, char *argv[])
{
    int opt, flagL = 0;
    while ((opt = getopt(argc, argv, "l")) != -1)
    {
        switch (opt)
        {
        case 'l':
            flagL = 1;
            break;
        default:
            fprintf(stderr, PROMPT_RED "Usage: %s [-l] [directory]\n" PROMPT_RESET, argv[0]);
            return EXIT_FAILURE;
        }
    }

    std::string dir_path = (optind < argc) ? argv[optind] : ".";

    DIR *dir = opendir(dir_path.c_str());
    if (dir == NULL)
    {
        perror("opendir");
        return EXIT_FAILURE;
    }

    std::vector<std::string> entries;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        entries.push_back(entry->d_name);
    }
    closedir(dir);

    std::sort(entries.begin(), entries.end());

    if (!flagL)
    {
        for (const auto &name : entries)
        {
            printf("%s ", name.c_str());
        }
        printf("\n");
    }
    else
    {
        for (const auto &name : entries)
        {
            std::string full_path = dir_path + "/" + name;
            struct stat st;
            if (stat(full_path.c_str(), &st) == -1)
            {
                perror("stat");
                continue;
            }

            // permissions
            char perm[11];
            perm[0] = S_ISDIR(st.st_mode) ? 'd' : (S_ISLNK(st.st_mode) ? 'l' : '-');
            perm[1] = st.st_mode & S_IRUSR ? 'r' : '-';
            perm[2] = st.st_mode & S_IWUSR ? 'w' : '-';
            perm[3] = st.st_mode & S_IXUSR ? 'x' : '-';
            perm[4] = st.st_mode & S_IRGRP ? 'r' : '-';
            perm[5] = st.st_mode & S_IWGRP ? 'w' : '-';
            perm[6] = st.st_mode & S_IXGRP ? 'x' : '-';
            perm[7] = st.st_mode & S_IROTH ? 'r' : '-';
            perm[8] = st.st_mode & S_IWOTH ? 'w' : '-';
            perm[9] = st.st_mode & S_IXOTH ? 'x' : '-';
            perm[10] = '\0';

            // links
            int links = st.st_nlink;

            // owner
            struct passwd *pw = getpwuid(st.st_uid);
            std::string owner = pw ? pw->pw_name : std::to_string(st.st_uid);

            // group
            struct group *gr = getgrgid(st.st_gid);
            std::string group = gr ? gr->gr_name : std::to_string(st.st_gid);

            // size
            long size = st.st_size;

            // time
            char time_str[20];
            struct tm *tm = localtime(&st.st_mtime);
            strftime(time_str, sizeof(time_str), "%b %d %H:%M", tm);

            // filename
            printf("%s %d %s %s %ld %s %s\n", perm, links, owner.c_str(), group.c_str(), size, time_str, name.c_str());
        }
    }

    return EXIT_SUCCESS;
}
