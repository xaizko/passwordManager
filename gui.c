#include <gtk/gtk.h>
#include "gui.h"
#include "encryption.h"

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Password Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    AppWidgets *widgets = g_malloc(sizeof(AppWidgets));

    widgets->stack = gtk_stack_new();
    gtk_window_set_child(GTK_WINDOW(window), widgets->stack);

    //Pages
    widgets->warning_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->login_page = gtk_grid_new();
    widgets->menu_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->add_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->delete_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->list_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->generate_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    
    //Add to stack
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->warning_page, "warning");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->login_page, "login");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->add_page, "add");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->delete_page, "delete");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->list_page, "list");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->generate_page, "generate");

    //Setup pages
    setup_warning_page(widgets);
    //setup_login_page(widgets);
    //setup_add_page(widgets);
    //setup_delete_page(widgets);
    //setup_list_page(widgets);
    //setup_generate_page(widgets);
    
    //Choose initial page
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/master.conf", getHomeEnv());
    if (!checkFileExist(fullpath)) {
	switch_page(widgets, "warning");
    } else {
	loginScreen(window);
    }
    gtk_window_present(GTK_WINDOW(window));

    return;
}

void switch_page(AppWidgets *widgets, const char *page_name) {
    gtk_stack_set_visible_child_name(GTK_STACK(widgets->stack), page_name);
}

void setup_warning_page(AppWidgets *widgets) {
    GtkWidget *warningBox = widgets->warning_page;
    
    gtk_widget_set_halign(warningBox, GTK_ALIGN_FILL);
    gtk_widget_set_valign(warningBox, GTK_ALIGN_FILL);

    GtkWidget *label = gtk_label_new("Please run './passwordManager --init' in the terminal before trying to use!");

    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(warningBox), label);

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

    LoginForm *form = g_new(LoginForm, 1);
    form->userInput = userInput;
    form->passInput = passInput;

    //submitButton
    submitButton = gtk_button_new_with_label("Log in");
    gtk_grid_attach(GTK_GRID(grid), submitButton, 1, 3, 1, 1);
    g_signal_connect(submitButton, "clicked", G_CALLBACK(validateLogin), form);
}

void validateLogin(GtkWidget *button, gpointer user_data) {
    LoginForm *form = (LoginForm *)user_data;
    const char *username = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->userInput)));
    const char *password = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->passInput)));

    int result = verifyCredentials((char *)username, (char *)password);
    
    if (result == 1) {
        g_print("Login successful!\n");
    } else {
        g_print("Login failed!\n");
    }

}


