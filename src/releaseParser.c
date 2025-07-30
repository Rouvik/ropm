#include <releaseParser.h>

int parseRelease(const char *filename, Release_t *release)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("[FATAL ERROR] Failed to open release file for parsing");
        fprintf(stderr, "[INFO] Release file path was: %s\n", filename);
        return 1;
    }

    size_t count;
    char *line = NULL;
    getline(&line, &count, fp);

    if (!count)
    {
        fputs("[ERROR] Failed to read \"maintainer\" info from Release, must be corrupted, please try again...\n", stderr);
        return 1;
    }

    release->maintainer = line;

    line = NULL;
    getline(&line, &count, fp);

    if (!count)
    {
        fputs("[ERROR] Failed to read \"version\" info from Release, must be corrupted, please try again...\n", stderr);
        free(line);
        return 1;
    }

    char *temp = line;

    char *dotPos = strchr(line, '.');
    if (!dotPos)
    {
        fprintf(stderr, "[ERROR] Maliformed version %s, aborting\n", line);
        free(temp);
        return 1;
    }

    (*dotPos) = 0;

    release->version[0] = atoi(line);

    line = dotPos + 1; // start from next char
    dotPos = strchr(line, '.');

    if (!dotPos)
    {
        fprintf(stderr, "[ERROR] Maliformed version %s, aborting\n", line);
        free(temp);
        return 1;
    }

    (*dotPos) = 0;
    release->version[1] = atoi(line);

    line = dotPos + 1;
    release->version[2] = atoi(line);

    free(temp);
    temp = NULL;
    line = NULL;

    getline(&line, &count, fp);

    if (!count)
    {
        fputs("[FATAL ERROR] Failed to read sha 256 checksum from Release\n", stderr);
        free(line);
        return 1;
    }

    release->sha256hash = line;

    return 0;
}

void freeRelease(Release_t *release)
{
    free(release->maintainer);
    free(release->sha256hash);
    release->maintainer = NULL;
    release->sha256hash = NULL;
    release->version[0] = release->version[1] = release->version[2] = 0;
}