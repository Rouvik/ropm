#include <shaCompare.h>

int sha256Compare(const char *SHA, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        perror("[FATAL ERROR] Failed to open file for SHA256 check");
        fprintf(stderr, "[INFO] SHA256 file open failed for: %s\n", filename);
        return -1;
    }
    
    unsigned char digest[EVP_MAX_MD_SIZE];

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx)
    {
        fputs("[FATAL ERROR] Failed to get a EVP context for hashing, aborting...\n", stderr);
        return -1;
    }
    
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1)
    {
        fputs("[FATAL ERROR] Failed to init EVP Digest for hashing, aborting...\n", stderr);
        return -1;
    }
    
    while (!feof(fp))
    {
        char buf[1024];
        int count = fread(buf, 1, 1024, fp);
        if (count)
        {
            EVP_DigestUpdate(mdctx, (void *)buf, count);
        }
    }

    fclose(fp);

    unsigned int digest_len;
    EVP_DigestFinal(mdctx, digest, &digest_len);
    EVP_MD_CTX_free(mdctx);

    char buf[digest_len * 2];
    for (int i = 0; i < digest_len; i++)
    {
        sprintf(buf + i * 2, "%02x", digest[i]);
    }
    
    return strncmp(SHA, buf, digest_len * 2) != 0;
}