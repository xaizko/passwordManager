#include "encryption.h"
#include "passwordManager.h"
#include <gtk/gtk.h>

// Forward declaration for LoginForm struct
typedef struct {
    GtkWidget *userInput;
    GtkWidget *passInput;
} LoginFormType;

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

//Validating input 
int verifyCredentials(char *username, char *password) {
    // Check credentials against stored values
    printf("Verifying credentials - username: %s\n", username);
    printf("Verifying credentials - password: [hidden]\n");

    // TODO: Implement actual credential verification logic
    return 0;
}
