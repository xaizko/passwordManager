#include "gui.h"

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);


    //check for master conf file
    char fullpath[512];
    char *home = getHomeEnv();
    snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/master.conf", home);
    if (!checkFileExist(fullpath)) {
	initWarning(window);
    } else {
	loginScreen(window);
    }
    gtk_window_present(GTK_WINDOW(window));

    return;
}

void initWarning(GtkWidget *window) {
    GtkWidget *box, *label;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(box, GTK_ALIGN_FILL);
    gtk_window_set_child(GTK_WINDOW(window), box);

    label = gtk_label_new("Please run './passwordManager --init' in the terminal before trying to use!");

    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), label);

} 

void loginScreen(GtkWidget *window) {
    GtkWidget *grid, *userLabel, *passLabel;
    GtkEntry *userInput, *passInput;
    GtkButton *submitButton;
}
