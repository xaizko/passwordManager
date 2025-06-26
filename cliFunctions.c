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

    //create encryption key
    char key[100];
    printf("Type another password or phrase that will be used for encryption when securing entries (Max of 99 characters): ");
    if (!fgets(key, sizeof(key), stdin)) {
	fprintf(stderr, "Failed to read key.\n");
	exit(EXIT_FAILURE);
    }
    //remove trialing new line
    password[strcspn(password, "\n")] = '\0';

    //hashes it to 32 bits
    char *hashedKey = hashText(key);

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

    //formatting path to keyFile
    char keyPath[512];
    snprintf(keyPath, sizeof(keyPath), "%s/%s/key.txt", home, CONFIG_PATH);

    //creating key file
    FILE *keyFile;
    keyFile = fopen(keyPath, "w");
    if (keyFile == NULL) {
	perror("Error creating key file");
	exit(EXIT_FAILURE);
    }

    //writes key to file
    char *hexKey = hashToHexUtility(hashedKey);
    fprintf(keyFile, "%s\n", hexKey);

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

    //free memory
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

 
