#include "encryption.h"

unsigned char *encryptText(char *textToEncrypt) {
    unsigned char *hash = malloc(SHA256_DIGEST_LENGTH);
    if (!hash) {
	fprintf(stderr, "Memory allocation failed.\n");
	exit(EXIT_FAILURE);
    }

    SHA256((unsigned char*)textToEncrypt, strlen(textToEncrypt), hash);
    
    return hash;
}
