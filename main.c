#include <stdio.h>
#include <string.h>

#include <util.h>
#include <commandHandler.h>
#include <setupHandler.h>
#include <installHandler.h>
#include <uninstallHandler.h>

void updateHandler(int, const char *argv[]);
void listHandler(int, const char *argv[]);

char const *options[] = {"setup", "install", "uninstall", "update", "list"};
void (*optionHandlers[])(int, const char *[]) = {setupHandler, installHandler, uninstallHandler, updateHandler, listHandler};

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("Error missing command parameters\nSyntax: %s [setup|install|update|list] [options...]\n", argv[0]);
        return 1;
    }

    if (!commandMenuExecute(argv[1], options, optionHandlers, argc, argv, ARRSIZE(options), false))
    {
        printf("Fatal error, no such option %s\n", argv[1]);
        return 1;
    }

    return 0;
}

void updateHandler(int, const char *argv[])
{
    printf("Updating...");
}

void listHandler(int, const char *argv[])
{
    printf("Listing...");
}