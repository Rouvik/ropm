#include <listHandler.h>

void listHandler(int argc, const char *argv[])
{
    const char *HOME = getenv("HOME");
    if (!HOME)
    {
        fputs("[FATAL ERROR] HOME environment variable missing, aborting\n", stderr);
        exit(1);
    }
    
    char buf[128];
    sprintf(buf, "%s/.ropm/Packages", HOME);

    FILE *fp = fopen(buf, "r");

    if (!fp)
    {
        fprintf(stderr, "[FATAL ERROR] Failed to open %s, make sure it exists, run \"%s setup\" to reinstall Package listing\n", buf, argv[0]);
        exit(1);
    }

    puts("Package\t| SHA256\n--------|-------");
    int count = 0;
    while (!feof(fp))
    {
        char *line = NULL;
        size_t size;
        getline(&line, &size, fp);

        if (!size)
        {
            free(line);
            continue;
        }
        
        char *sp = strchr(line, ' ');
        if (!sp)
        {
            fputs("[ERROR] Maliformed package string, probably package maintainers fault or tampering with .ropm/Packages by user, ignoring entry\n", stderr);
            continue;
        }
        
        *sp = 0;        // make the space a null character to split the string in 2 parts
        *(sp + 7) = 0;  // make the 6th SHA character after space a null to extract the first 5 chars of the SHA
        printf("%s\t| %s\n", line, sp + 1);

        free(line);
        count++;
    }
    
    printf("Total %d packages in remote, if you don\'t find an existing package then run \"%s setup\" to reset the package ring (this won\'t delete your existing programs)\n", count, argv[0]);
}