#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/rand.h>
#include <glib.h>

#define AES_KEYLEN 32 // 256 BITS
#define AES_IVLEN 16 // 128 bits

//Encryption functions
unsigned char *hashText(const char *textToEncrypt);
char *encryptText(char *textToEncrypt, char *aesKey);
char *decryptText(char *textToDecrypt, unsigned char *aesKey);

//Utility functions
int verifyCredentials(char *username, char* password);
int verifyPassword(const char *password);
void hashToHex(FILE *ftpr, unsigned char hashed[]);
char *hashToHexUtility(unsigned char *hash);
char *base64_encode(const unsigned char *buffer, size_t length);
unsigned char *retrieveDecryptedAESKey(const char *password);

#endif // ENCRYPTION_H
