#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdio.h>
#include <openssl/sha.h>

//Encryption functions
unsigned char *encryptText(char *textToEncrypt);

//Utility functions
int verifyCredentials(char *username, char* password);
void hashToHex(FILE *ftpr, unsigned char hashed[]);
char *hashToHexUtility(unsigned char *hash);

#endif // ENCRYPTION_H
