#ifndef INCLUDED_SHA_COMPARE_H
#define INCLUDED_SHA_COMPARE_H

#include <string.h>

#include <openssl/evp.h>
#include <openssl/sha.h>

/**
 * @brief Compares the provided SHA string with the sha generated from a file from filename path
 * 
 * @param SHA The SHA string to compare to
 * @param filename The filename to generate SHA for and compare
 * @return 0 = success | -1 = error | 1 = SHA check failed, no match
 */
int sha256Compare(const char *SHA, const char *filename);

#endif // INCLUDED_SHA_COMPARE_H