#include <stdio.h>
#include <string.h>

#include <util.h>
#include <commandHandler.h>
#include <setupHandler.h>
#include <installHandler.h>
#include <uninstallHandler.h>
#include <listHandler.h>

void updateHandler(int argc, const char *argv[]);

char const *options[] = {"setup", "install", "xinstall", "uninstall", "update", "xupdate", "list"};
void (*optionHandlers[])(int, const char *[]) = {setupHandler, installHandler, installHandler, uninstallHandler, updateHandler, updateHandler, listHandler};

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("Error missing command parameters\nSyntax: %s [setup|install|xinstall|uninstall|update|list] [options...]\n", argv[0]);
        return 1;
    }

    if (!commandMenuExecute(argv[1], options, optionHandlers, argc, argv, ARRSIZE(options), false))
    {
        printf("Fatal error, no such option %s\n", argv[1]);
        return 1;
    }

    return 0;
}

void updateHandler(int argc, const char *argv[])
{
    char *t = "xinstall";

    if (argv[1][0] == 'x')  // run in unsafe mode
    {
        char ch = 'n';
        fputs("Run in uncheck mode? This won\'t check the package SHA256, use with caution (y/n[default]): ", stdout);
        scanf("%c", &ch);
        if (ch == 'n')
        {
            puts("Got it aborting...");
            exit(0);
        }

        argv[1] = t;   // set install mode to xinstall
    }

    uninstallHandler(argc, argv);
    installHandler(argc, argv);
}