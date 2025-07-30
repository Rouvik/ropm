#include <installHandler.h>

void installHandler(int argc, const char *argv[])
{
    if (argc < 2)
    {
        fputs("Error, install command requires the <package> argument\nSyntax: ropm install <package>\n", stderr);
        exit(1);
    }

    char *HOME = getenv("HOME");
    if (!HOME)
    {
        fputs("[FATAL ERROR] No HOME environment variable set, required for finding Packages.gz listing\n", stderr);
        exit(1);
    }

    char buf[128];
    sprintf(buf, "%s/.ropm/Packages", HOME);

    const char *packageName = argv[2];

    FILE *fp = fopen(buf, "r");
    if (!fp)
    {
        perror("[FATAL ERROR] Failed to open file");
        fprintf(stderr, "[INFO] File path for packages was: %s\n", buf);
        exit(1);
    }

    while (!feof(fp))
    {
        char *line = NULL;
        size_t count;
        getline(&line, &count, fp);

        if (!count)
        {
            continue;
        }

        char *sp = strchr(line, ' ');
        if (!sp)
        {
            fprintf(stderr, "[ERROR] Maliformed entry in Packages skipping: %s\n", line);
            free(line);
            continue;
        }

        (*sp) = 0;
        char *sha = sp + 1;

        if (!strstr(line, packageName))
        {
            free(line);
            continue;
        }

        if (strcmp(line, packageName))
        {
            printf("[INFO] Similar named package detected: %s\n", line);
            free(line);
            continue;
        }

        puts("[INFO] Package found, downloading...");
        sprintf(buf, "curl " downloadRepoURL "%s.tar.gz -o %s/.ropm/%s.tar.gz", packageName, HOME, packageName);
        if (system(buf))
        {
            fprintf(stderr, "[FATAL ERROR] Curl failed download from repo aborting...\nCommand: %s\n", buf);
            free(line);
            fclose(fp);
            exit(1);
        }

        sprintf(buf, "%s/.ropm/%s.tar.gz", HOME, packageName);
        if (sha256Compare(sha, buf))
        {
            fputs("[FATAL ERROR] SHA check failed aborting, package is probably compromised\n", stderr);
            free(line);
            sprintf(buf, "%s/.ropm/%s.tar.gz", HOME, packageName);
            remove(buf);
            fclose(fp);
            exit(1);
        }

        free(line);
        line = NULL;
        sha = NULL;
        sp = NULL;

        puts("[INFO] Package SHA match...installing");

        sprintf(buf, "gzip -d %s/.ropm/%s.tar.gz", HOME, packageName);
        if (system(buf))
        {
            fputs("[FATAL ERROR] Failed to decompress package\n", stderr);
            sprintf(buf, "%s/.ropm/%s.tar", HOME, packageName);
            remove(buf);
            fclose(fp);
            exit(1);
        }

        sprintf(buf, "tar -xf %s/.ropm/%s.tar -C %s/.ropm", HOME, packageName, HOME);
        if (system(buf))
        {
            fputs("[FATAL ERROR] Failed to extract package\n", stderr);
            sprintf(buf, "%s/.ropm/%s.tar", HOME, packageName);
            remove(buf);
            fclose(fp);
            exit(1);
        }

        sprintf(buf, "%s/.ropm/%s.tar", HOME, packageName);
        remove(buf);

        sprintf(buf, "make -C %s/.ropm/%s && make -C %s/.ropm/%s install", HOME, packageName, HOME, packageName);
        if (system(buf))
        {
            fputs("[FATAL ERROR] Failed to build and install package\n", stderr);
            fclose(fp);
            exit(1);
        }

        puts("[INFO] Install complete leaving...");

        sprintf(buf, "rm -rf %s/.ropm/%s", HOME, packageName);
        if (system(buf))
        {
            fprintf(stderr, "[ERROR] Failed to delete installation files, try removing manually .ropm/%s\n", packageName);
            fclose(fp);
            exit(1);
        }

        fclose(fp);
        return; // stop installation complete!
    }

    fprintf(stderr, "[FATAL ERROR] Failed to find package: %s installation aborted\n", packageName);
    fclose(fp);
    exit(1);
}