#include <uninstallHandler.h>

void uninstallHandler(int argc, const char *argv[])
{
    char *HOME = getenv("HOME");
    if (!HOME)
    {
        fputs("[FATAL ERROR] Missing environment variable HOME aborting\n", stderr);
        exit(1);
    }

    if (argc < 2)
    {
        fputs("Error, uninstall requires the package argument to remove ropm uninstall <package>\n", stderr);
        exit(1);
    }

    const char *packageName = argv[2];

    char buf[128];

    struct dirent *entry;
    DIR *dir;

    sprintf(buf, "%s/.ropm/bin", HOME);

    dir = opendir(buf);
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, packageName) == 0)
        {
            sprintf(buf, "make -C %s/.ropm/bin/%s.build uninstall", HOME, packageName);
            if (system(buf))
            {
                perror("[FATAL ERROR] Failed to uninstall package, aborting, please try again or remove manually from ~/.ropm/bin");
                closedir(dir);
                exit(1);
            }

            puts("[INFO] Uninstalled package successfully");
            return;
        }
    }

    fprintf(stderr, "[ERROR] No such package found: %s please use exact package name to remove\n", packageName);
    exit(1);
}