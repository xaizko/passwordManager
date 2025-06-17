#include "cliFunctions.h"
#include "passwordManager.h"

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
