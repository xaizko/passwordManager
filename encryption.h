#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>

#define AES_KEYLEN 32 // 256 BITS
#define AES_IVLEN 16 // 128 bits

extern const unsigned char *IVKey;

//Encryption functions
unsigned char *hashText(char *textToEncrypt);
char *encryptText(char *textToEncrypt);
char *decryptText(char *textToDecrypt);

//Utility functions
int verifyCredentials(char *username, char* password);
void hashToHex(FILE *ftpr, unsigned char hashed[]);
char *hashToHexUtility(unsigned char *hash);
char *base64_encode(char *buffer, size_t length);
char *retrieveKey();

#endif // ENCRYPTION_H
