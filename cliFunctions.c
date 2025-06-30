#include "cliFunctions.h"

void printHelp() {
    printf(
	"┌────────────────────────────────────────────────────────────┐\n"
	"│                    Password Manager Help                   │\n"
	"└────────────────────────────────────────────────────────────┘\n"		
	"\n"
	"USAGE:\n"
	"  ./passwordManager --help\n"
	"\n"
	"DESCRIPTION:\n"
	"  This is a secure password manager.\n"
	"\n"
	"  To get started, you must initialize your vault by setting a\n"
	"  master password and username.\n"
	"\n"
	"┌────────────────────────────────────────────────────────────┐\n"
	"│                    INITIAL SETUP REQUIRED                  │\n"
	"└────────────────────────────────────────────────────────────┘\n"
	"\n"
	"Run the follow command to begin setup:\n"
	"\n"
	"  ./passwordManager --init\n"
	"\n"
	"This will prompt you to craete a new username and master password.\n"
	"\n"
	"Use './passwordManager --help' anytime to see this menu again.\n"
    );

    return;
}

void initSetup() {
    char *home = getHomeEnv();

    //check if setup has already been done
    char masterLoginPath[512];
    snprintf(masterLoginPath, sizeof(masterLoginPath), "%s/%s/master.conf", home, CONFIG_PATH);
    if (checkFileExist(masterLoginPath)) {
	fprintf(stderr, "Master set up already done. Run the program with just ./passwordManager\n");
	exit(EXIT_FAILURE);
    }

    //create directory to store everything
    create_config_dir();

    //print warning
    printf("%sNOTE: Please do not forget any of the information you type otherwise it will be very hard to retrieve it.%s\n", AC_RED, AC_NORMAL);

    //get master username
    char username[21];
    printf("Type your username (Max of 20 characters): ");
    if(!fgets(username, sizeof(username), stdin)) {
	fprintf(stderr, "Failed to read username.\n");
	exit(EXIT_FAILURE);
    }
    //removes trailing new line
    username[strcspn(username, "\n")] = '\0';

    //encrypt username
    char *hashedUsername = hashText(username);

    //get master password
    char password[51];
    printf("Type your password (Max of 50 characters): ");
    if (!fgets(password, sizeof(password), stdin)) {
	fprintf(stderr, "Failed to read username.\n");
	exit(EXIT_FAILURE);
    }
    //removes trailing new line
    password[strcspn(password, "\n")] = '\0';

    //encrypts password
    char *hashedPassword = hashText(password); 

    //creates master file to store master password and user
    FILE *masterConfig;
    masterConfig = fopen(masterLoginPath, "w");
    if (masterConfig == NULL) {
	perror("Error creating master config file");
	exit(EXIT_FAILURE);
    } else {
	printf("Successfully created master config file! Please run the program normally and log in\n");
    }
    
    //converts raw hash to hex for easy reversability
    hashToHex(masterConfig, hashedUsername);
    hashToHex(masterConfig, hashedPassword);

    //formatting path for fopen
    char storagePath[512];
    snprintf(storagePath, sizeof(storagePath), "%s/%s/storage.db", home, CONFIG_PATH);

    //creating storage file 
    FILE *masterStorage;
    masterStorage = fopen(storagePath, "w");
    if (masterStorage == NULL) {
	perror("Error creating database file");
	exit(EXIT_FAILURE);
    }

    // --- KEY SETUP STARTS HERE ---

    //generate salt
    unsigned char salt[16];
    if (!RAND_bytes(salt, sizeof(salt))) {
	fprintf(stderr, "Error generating salt\n");
	exit(EXIT_FAILURE);
    }

    //derive key from passwordManager
    unsigned char derivedKey[32];
    if (!PKCS5_PBKDF2_HMAC(password, strlen(password), salt, sizeof(salt), 100000, EVP_sha256(), sizeof(derivedKey), derivedKey)) {
	fprintf(stderr, "PBKDF2 failed \n");
	exit(EXIT_FAILURE);
    }

    //key that will be using for encryption
    unsigned char aesKey[32];
    if (!RAND_bytes(aesKey, sizeof(aesKey))) {
	fprintf(stderr, "Failed to generate entry encryption key\n");
	exit(EXIT_FAILURE);
    }

    //generate random IV for encrypting entry key
    unsigned char iv[16];
    if (!RAND_bytes(iv, sizeof(iv))) {
	fprintf(stderr, "Failed to generate IV\n");
    }

    //encrypt aesKey with derived key
    unsigned char encryptedAesKey[48];
    int len = 0, ciphertext_len = 0;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, derivedKey, iv);
    EVP_EncryptUpdate(ctx, encryptedAesKey, &len, aesKey, sizeof(aesKey));
    ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, encryptedAesKey + len, &len);
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    //Save salt, iv, and encrypted key 
    char keyPath[512];
    snprintf(keyPath, sizeof(keyPath), "%s/%s/key.enc", home, CONFIG_PATH);
    FILE *keyFile = fopen(keyPath, "wb");
    if (!keyFile) {
	perror("Error creating key file");
	exit(EXIT_FAILURE);
    }
    fwrite(salt, 1, sizeof(salt), keyFile);
    fwrite(iv, 1, sizeof(iv), keyFile);
    fwrite(encryptedAesKey, 1, ciphertext_len, keyFile);

    //free memory
    fclose(keyFile);
    fclose(masterConfig);
    fclose(masterStorage);
    free(hashedUsername);
    free(hashedPassword);

    return;
}

//check if the config file exists
int checkFileExist(char *path) {
    return access(path, F_OK) != -1;
}

//creates directory for config 
void create_config_dir() {
    //get home directory
    char *home = getHomeEnv();

    //builds full string path because mkdir does not support %s
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", home, CONFIG_PATH);

    //0700 makes the directory accessible only to the user
    if (mkdir(path, 0700) == -1) {
	if (errno != EEXIST) { // only fails if it just just that the dir already exists
	    perror("Failed to initialize passwordManager directory");
	    exit(EXIT_FAILURE);
	}
    }
}

//returns the home directory
char *getHomeEnv() {
    char *home = getenv("HOME");
    if (home == NULL) {
	fprintf(stderr, "Could not get HOME environment.\n");
	exit(EXIT_FAILURE);
    }
    return home;
}

 
