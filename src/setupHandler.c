#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include <setupHandler.h>
#include <repoDownload.h>

void setupHandler(int argc, const char *argv[])
{
    struct stat sf = {0};
    const char *HOME = getenv("HOME");
    char buf[256];
    sprintf(buf, "%s/.ropm", HOME);

    if (stat(buf, &sf) == -1)
    {
        if (mkdir(buf, S_IRWXU) == -1)
        {
            perror("[FATAL ERROR] Failed to create .ropm in $HOME");
            exit(1);
        }
    }

    sprintf(buf, "%s/.ropm/bin", HOME);
    if (stat(buf, &sf) == -1)
    {
        if (mkdir(buf, S_IRWXU) == -1)
        {
            perror("[FATAL ERROR] Failed to create .ropm/bin in $HOME");
            exit(1);
        }
    }

    sprintf(buf, "curl " downloadRepoURL "Release -o %s/.ropm/Release", HOME);
    if (system(buf))
    {
        fputs("[FATAL ERROR] Failed to download Release from remote repo, please check the internet connection and try setup again", stderr);
        exit(1);
    }

    sprintf(buf, "curl " downloadRepoURL "Release.asc -o %s/.ropm/Release.asc", HOME);
    if (system(buf))
    {
        fputs("[FATAL ERROR] Failed to download Release.asc from remote repo, please check the internet connection and try setup again", stderr);
        exit(1);
    }

    char opt = 'y';
    printf("Perform keybased checks on import? [Use n if key import before failed, default is y] (y/n) :: ");
    scanf("%c", &opt);

    if (opt == 'y')
    {
        if (system("gpg --import pubkey.asc"))
        {
            fputs("[FATAL ERROR] Failed to import public key from pubkey.asc, safe package imports may not work", stderr);
            exit(1);
        }

        sprintf(buf, "gpg --verify %s/.ropm/Release.asc %s/.ropm/Release", HOME, HOME);
        if (system(buf))
        {
            fputs("[FATAL ERROR] GPG verification failed, either the gpg program is not installed or the Release is corrupted, please restart again or use a safer installation!\nDeleting malicious Release files\n", stderr);

            sprintf(buf, "%s/.ropm/Release", HOME);
            remove(buf);

            sprintf(buf, "%s/.ropm/Release.asc", HOME);
            remove(buf);

            exit(1);
        }

        puts("[INFO] Release validation successfull!");
        sprintf(buf, "curl " downloadRepoURL "Packages.gz -o %s/.ropm/Packages.gz", HOME);
        if (system(buf))
        {
            fputs("[FATAL ERROR] Failed to download Packages.gz from repo, please try again with proper internet settings\n", stderr);
            exit(1);
        }

        sprintf(buf, "%s/.ropm/Release", HOME);
        Release_t release;
        if (parseRelease(buf, &release))
        {
            fputs("[FATAL ERROR] Failed to parse release\n", stderr);
            freeRelease(&release);
            exit(1);
        }

        printf("[DEBUG] Release:\nMaintainer: %s\nVersion: %d.%d.%d\nSHA256: %s\n", release.maintainer, release.version[0], release.version[1], release.version[2], release.sha256hash);

        sprintf(buf, "%s/.ropm/Packages.gz", HOME);
        if (sha256Compare(release.sha256hash, buf)) // sha check failed
        {
            fputs("[FATAL ERROR] SHA checksum didnt match with the Packages.gz, must be compromised aborting\n", stderr);
            remove(buf);
            exit(1);
        }

        freeRelease(&release);

        sprintf(buf, "gzip -d %s/.ropm/Packages.gz", HOME);
        if (system(buf))
        {
            fputs("[FATAL ERROR] Failed to extract Packages.gz", stderr);
            exit(1);
        }

        puts("Release and Package listing updated successfully use ropm install or ropm install-x (insecure) to install packages");
    }
    else
    {
        sprintf(buf, "curl " downloadRepoURL "Packages.gz -o %s/.ropm/Packages.gz", HOME);
        if (system(buf))
        {
            fputs("[FATAL ERROR] Failed to download Packages.gz from repo, please try again with proper internet settings\n", stderr);
            exit(1);
        }

        sprintf(buf, "gzip -d %s/.ropm/Packages.gz", HOME);
        if (system(buf))
        {
            fputs("[FATAL ERROR] Failed to extract Packages.gz", stderr);
            exit(1);
        }

        puts("Release and Package listing updated successfully use ropm install or ropm install-x (insecure) to install packages");
    }
}