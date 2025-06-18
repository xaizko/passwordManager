#include "gui.h"

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;

    //check for master conf file
    char fullpath[512];
    char *home = getHomeEnv();
    snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/master.conf", home);

    //if (checkFileExist(fullpath)) {
    return;
}
