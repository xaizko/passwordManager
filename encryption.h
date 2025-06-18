#include "passwordManager.h"

//Encryption Library
#include <openssl/sha.h>

//Encryption functions
unsigned char *encryptText(char *textToEncrypt);

//Utility functions
void hashToHex(FILE *ftpr, unsigned char hashed[]);
