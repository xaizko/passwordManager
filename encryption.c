#include "encryption.h"

//hashes text
unsigned char *encryptText(char *textToEncrypt) {
    unsigned char *hash = malloc(SHA256_DIGEST_LENGTH);
    if (!hash) {
	fprintf(stderr, "Memory allocation failed.\n");
	exit(EXIT_FAILURE);
    }

    SHA256((unsigned char*)textToEncrypt, strlen(textToEncrypt), hash);
    
    return hash;
}

//write hash as hex
void hashToHex(FILE *ftpr, unsigned char hashed[]) {
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
	fprintf(ftpr, "%02x", hashed[i]);
    }
    fprintf(ftpr, "\n");
}
