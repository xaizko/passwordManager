#include "encryption.h"
#include "passwordManager.h"
#include <gtk/gtk.h>

// Forward declaration for LoginForm struct
typedef struct {
    GtkWidget *userInput;
    GtkWidget *passInput;
} LoginFormType;

//hashes text
unsigned char *hashText(char *textToEncrypt) {
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

char *hashToHexUtility(unsigned char *hash) {
    char *hex = malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (!hex) {
	return NULL;
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
	snprintf(hex + (i*2), 3, "%02x", hash[i]);
    }
    hex[SHA256_DIGEST_LENGTH * 2] = '\0';

    return hex;
}

//Validating input 
int verifyCredentials(char *username, char *password) {
    unsigned char *rawUser = encryptText(username);
    char *hashedUser = hashToHexUtility(rawUser);
    free(rawUser);

    unsigned char *rawPass = encryptText(password);
    char *hashedPassword = hashToHexUtility(rawPass);
    free(rawPass);

    char *login[2] = {hashedUser, hashedPassword};

    //path to master conf file
    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/master.conf", getenv("HOME"));
    FILE *masterConf = fopen(fullpath, "r");

    //buffer for reading -- reads 66, chars 64 for hash 1 for \n and \0
    char line[SHA256_DIGEST_LENGTH * 2 + 2];
    int index = 0;

    while(fgets(line, sizeof(line), masterConf)) {
	line[strcspn(line, "\n")] = 0; //remove new line
	
	if (index < 2 && strcmp(login[index], line) == 0) {
	    index++;
	} else {
	    break;
	}
    }

    fclose(masterConf);

    //frees from hashToHexUtility
    free(hashedUser);
    free(hashedPassword);

    return (index == 2); 
}

