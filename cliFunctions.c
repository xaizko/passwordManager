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
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/master.conf", home);
    if (checkFileExist(fullpath)) {
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


    //get master password
    char password[51];
    printf("Type your password (Max of 50 characters): ");
    if (!fgets(password, sizeof(password), stdin)) {
	fprintf(stderr, "Failed to read username.\n");
	exit(EXIT_FAILURE);
    }
    //removes trailing new line
    password[strcspn(password, "\n")] = '\0';

    char *hashedPassword = encryptText(password); 

    free(hashedPassword);
    

    //creates master file to store master password and user
    FILE *masterConfig;
    masterConfig = fopen(fullpath, "w");
    if (masterConfig == NULL) {
	perror("Error creating master config file");
	exit(EXIT_FAILURE);
    } else {
	printf("Successfully created master config file! Please run the program normally and log in\n");
    }

    fclose(masterConfig);
    
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
