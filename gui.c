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
    GtkWidget *userInput, *passInput;
    GtkWidget *submitButton;

    //creates grid
    grid = gtk_grid_new();
    gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
    gtk_widget_set_valign(grid, GTK_ALIGN_FILL);
    gtk_window_set_child(GTK_WINDOW(window), grid);

    //create uername 
    userLabel = gtk_label_new("Username");
    gtk_widget_set_halign(userLabel, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(userLabel, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), userLabel, 1, 1, 1, 1);

    //create password
    passLabel = gtk_label_new("Password");
    gtk_grid_attach(GTK_GRID(grid), passLabel, 1, 2, 1, 1);

    //username entry
    userInput = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(userInput), "Guest");
    gtk_grid_attach_next_to(GTK_GRID(grid), userInput, userLabel, GTK_POS_RIGHT, 1, 1);

    //password entry
    passInput = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(passInput), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(passInput), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(passInput), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_invisible_char(GTK_ENTRY(passInput), '*');
    gtk_grid_attach_next_to(GTK_GRID(grid), passInput, passLabel, GTK_POS_RIGHT,1 ,1);

    //submitButton
    submitButton = gtk_button_new_with_label("Log in");
    gtk_grid_attach(GTK_GRID(grid), submitButton, 1, 3, 1, 1);
}

int validateLogin() {

}
