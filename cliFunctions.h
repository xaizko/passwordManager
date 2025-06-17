#include "passwordManager.h"

#define CONFIG_PATH ".config/passwordManager"

//help menu
void printHelp();

//init functions
int checkFileExist(char *path);
void create_config_dir();
void initSetup();

//misc utility
char *getHomeEnv();
