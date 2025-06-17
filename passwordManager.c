#include "passwordManager.h"
#include "gui.h"

int main (int argc,char **argv) {
    int status;
    GtkApplication *app;

    app = gtk_application_new ("com.github.xaizko.passwordManager", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    printf("Hello world\n");
    printf("This is a test\n");
    return status;
}

