#ifndef CLI_FUNCTIONS_H
#define CLI_FUNCTIONS_H

#include "passwordManager.h"
#include "encryption.h"

#define CONFIG_PATH ".config/passwordManager"

//help menu
void printHelp();

//init functions
int checkFileExist(char *path);
void create_config_dir();
void initSetup();

//misc utility
char *getHomeEnv();

#endif // CLI_FUNCTIONS_H
