#include "passwordManager.h"
#include "gui.h"
#include "cliFunctions.h"

int main (int argc,char **argv) {

    if (argc > 1) {
	if (!strcmp(argv[1], "--help")) {
	    printHelp();
	} else if (!strcmp(argv[1], "--init")) {
	    initSetup();
	}
    } 

    //int status;
    //GtkApplication *app;

    //app = gtk_application_new ("com.github.xaizko.passwordManager", G_APPLICATION_DEFAULT_FLAGS);
    //g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    //status = g_application_run (G_APPLICATION (app), argc, argv);
    //g_object_unref (app);

    //return status;
    return 0;
}

