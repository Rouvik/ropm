#ifndef INCLUDED_RELEASE_PARSER_H
#define INCLUDED_RELEASE_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Release
{
    char *maintainer;
    uint8_t version[3];
    char *sha256hash;    // 64 hex chars + 1 null char
} Release_t;

int parseRelease(const char *filename, Release_t *release);

void freeRelease(Release_t *release);

#endif // INCLUDED_RELEASE_PARSER_H