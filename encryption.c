#include "encryption.h"
#include "passwordManager.h"
#include <gtk/gtk.h>

// Forward declaration for LoginForm struct
typedef struct {
    GtkWidget *userInput;
    GtkWidget *passInput;
} LoginFormType;

//converts binary data to base64 text
char *base64_encode(char *buffer, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // ignores new lines
    BIO_write(b64, buffer, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bufferPtr); 

    char *b64text = malloc(bufferPtr->length + 1);
    memcpy(b64text, bufferPtr->data, bufferPtr->length);
    b64text[bufferPtr->length] = '\0';

    BIO_free_all(b64);
    return b64text;
}

char *encryptText(char *textToEncrypt, char *aesKey) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int len, ciphertext_len;
    int plaintext_len = strlen(textToEncrypt);

    unsigned char IVKey[16];
    RAND_bytes(IVKey, sizeof(IVKey));
    
    char ciphertext[1040];
    char combined[1056];

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aesKey, IVKey);
    EVP_EncryptUpdate(ctx, ciphertext, &len, (char *)textToEncrypt, plaintext_len);
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    //combine iv and cipher text
    memcpy(combined, iv, 16);
    memcpy(combined + 16, ciphertext, ciphertext_len);

    return base64_encode((char *)combined, ciphertext_len + 16);
}

char *decryptText(char *textToDecrypt) {

}

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
    unsigned char *rawUser = hashText(username);
    char *hashedUser = hashToHexUtility(rawUser);
    free(rawUser);

    unsigned char *rawPass = hashText(password);
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

char *retrieveKey() {
    FILE *keyFile;
    char keyPath[512]; 
    snprintf(keyPath, sizeof(keyPath), "%s/.config/passwordManager/key.txt", getenv("HOME"));
    
    keyFile = fopen(keyPath, "r");
    if (!keyFile) {
	perror("Failed to open key file");
	return NULL;
    }
    
    char temp[1024];
    if (!fgets(temp, sizeof(temp), keyFile)) {
	perror("Failed to read key");
	fclose(keyFile);
	return NULL;
    }
    fclose(keyFile);
    
    //remove trailing new line
    size_t len = strlen(temp);
    if (len > 0 && temp[len -1] == '\n') {
	temp[len - 1] = '\0';
    }

    char *key = malloc(strlen(temp) + 1);
    if (!key) return NULL;
    strcpy(key, temp);
    
    return key; 

} 
