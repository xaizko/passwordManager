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
    widgets->menu_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->login_page = gtk_grid_new();
    widgets->add_page = gtk_grid_new();
    widgets->delete_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->list_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    widgets->generate_page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);

    //Set up entries
    
    //Login entries
    widgets->userInput = gtk_entry_new();
    widgets->passInput = gtk_entry_new();

    //Add page entries
    widgets->appInput = gtk_entry_new(); 
    widgets->addUserInput = gtk_entry_new();
    widgets->addPassInput = gtk_entry_new();
    widgets->accPassInput = gtk_entry_new();

    //List page entries
    widgets->listPassInput = gtk_entry_new();

    //Add to stack
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->warning_page, "warning");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->menu_page, "menu");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->login_page, "login");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->add_page, "add");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->delete_page, "delete");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->list_page, "list");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->generate_page, "generate");

    //Setup pages
    setup_warning_page(widgets);
    setup_menu_page(widgets);
    setup_login_page(widgets);
    setup_add_page(widgets);
    //setup_delete_page(widgets);
    setup_list_page(widgets);
    //setup_generate_page(widgets);
    
    //Choose initial page
    char fullpath[512];
    snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/master.conf", getHomeEnv());
    if (!checkFileExist(fullpath)) {
	switch_page(widgets, "warning");
    } else {
	switch_page(widgets, "login");
    }
    gtk_window_present(GTK_WINDOW(window));

    return;
}

//switches initial login page
void switch_page(AppWidgets *widgets, const char *page_name) {
    gtk_stack_set_visible_child_name(GTK_STACK(widgets->stack), page_name);
}

//switches pages based on name set up in activate function
void handle_page_switch(GtkButton *button, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    const char *page_name = g_object_get_data(G_OBJECT(button), "page");
    if (page_name) {
	switch_page(widgets, page_name);
    } else{
	g_warning("No page found with that name!");
    }
}

//builds the warning page
void setup_warning_page(AppWidgets *widgets) {
    GtkWidget *warningBox = widgets->warning_page;
    
    gtk_widget_set_halign(warningBox, GTK_ALIGN_FILL);
    gtk_widget_set_valign(warningBox, GTK_ALIGN_FILL);

    GtkWidget *label = gtk_label_new("Please run './passwordManager --init' in the terminal before trying to use!");

    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(warningBox), label);

} 

//builds the menu page
void setup_menu_page(AppWidgets *widgets) {
    GtkWidget *menu = widgets->menu_page;

    //create buttons
    GtkWidget *add_btn = gtk_button_new_with_label("Add Password");
    GtkWidget *del_btn = gtk_button_new_with_label("Delete Password");
    GtkWidget *list_btn = gtk_button_new_with_label("List Passwords");
    GtkWidget *gen_btn = gtk_button_new_with_label("Generate Pssword");

    //Add to box
    gtk_box_append(GTK_BOX(menu), add_btn);
    gtk_box_append(GTK_BOX(menu), del_btn);
    gtk_box_append(GTK_BOX(menu), list_btn);
    gtk_box_append(GTK_BOX(menu), gen_btn);

    //Navigation hooks
    g_object_set_data(G_OBJECT(add_btn), "page", "add");
    g_signal_connect(add_btn, "clicked", G_CALLBACK(handle_page_switch), widgets);

    g_object_set_data(G_OBJECT(del_btn), "page", "delete");
    g_signal_connect(del_btn, "clicked", G_CALLBACK(handle_page_switch), widgets);

    g_object_set_data(G_OBJECT(list_btn), "page", "list");
    g_signal_connect(list_btn, "clicked", G_CALLBACK(handle_page_switch), widgets);

    g_object_set_data(G_OBJECT(gen_btn), "page", "generate");
    g_signal_connect(gen_btn, "clicked", G_CALLBACK(handle_page_switch), widgets);
}

// builds the login page
void setup_login_page(AppWidgets *widgets) {
    GtkWidget *userLabel, *passLabel;
    GtkWidget *submitButton;
    GtkWidget *userInput, *passInput;

    //creates grid
    GtkWidget *grid = widgets->login_page; 
    gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
    gtk_widget_set_valign(grid, GTK_ALIGN_FILL);

    //create username 
    userLabel = gtk_label_new("Username");
    gtk_widget_set_halign(userLabel, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(userLabel, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), userLabel, 1, 1, 1, 1);

    //create password
    passLabel = gtk_label_new("Password");
    gtk_grid_attach(GTK_GRID(grid), passLabel, 1, 2, 1, 1);

    //username entry
    userInput = widgets->userInput;
    gtk_entry_set_placeholder_text(GTK_ENTRY(userInput), "Guest");
    gtk_grid_attach_next_to(GTK_GRID(grid), userInput, userLabel, GTK_POS_RIGHT, 1, 1);

    //password entry
    passInput = widgets->passInput;
    gtk_entry_set_placeholder_text(GTK_ENTRY(passInput), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(passInput), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(passInput), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_invisible_char(GTK_ENTRY(passInput), '*');
    gtk_grid_attach_next_to(GTK_GRID(grid), passInput, passLabel, GTK_POS_RIGHT,1 ,1);

    //populating data to send
    LoginForm *form = g_new(LoginForm, 1);
    form->userInput = userInput;
    form->passInput = passInput;
    form->widgets = widgets;

    //submitButton
    submitButton = gtk_button_new_with_label("Log in");
    gtk_grid_attach(GTK_GRID(grid), submitButton, 1, 3, 1, 1);
    g_signal_connect(submitButton, "clicked", G_CALLBACK(validateLogin), form);
}

//necessary for checking login 
void validateLogin(GtkWidget *button, gpointer user_data) {
    LoginForm *form = (LoginForm *)user_data;
    const char *username = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->userInput)));
    const char *password = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->passInput)));
    AppWidgets *widgets = form->widgets;

    int result = verifyCredentials((char *)username, (char *)password);
    
    if (result == 1) {
        switch_page(widgets, "menu");
    } else {
        g_print("Login failed!\n");
    }
}

//setup for adding passwords
void setup_add_page(AppWidgets *widgets) {
    //set up grid
    GtkWidget *grid = widgets->add_page;
    gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
    gtk_widget_set_valign(grid, GTK_ALIGN_FILL);

    //set up labels
    GtkWidget *appLabel = gtk_label_new("Application");
    gtk_widget_set_halign(appLabel, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(appLabel, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(grid), appLabel, 1, 1, 1, 1);

    //user label
    GtkWidget *userLabel = gtk_label_new("Username");
    gtk_grid_attach(GTK_GRID(grid), userLabel, 1, 2, 1, 1);

    //password label
    GtkWidget *passLabel = gtk_label_new("Password");
    gtk_grid_attach(GTK_GRID(grid), passLabel, 1, 3, 1, 1);

    //Acc password label
    GtkWidget *accPassLabel = gtk_label_new("Account Password (Needed for encryption)");
    gtk_grid_attach(GTK_GRID(grid), accPassLabel, 1, 4, 1, 1);

    //set up entries
    GtkWidget *userInput, *passInput, *appInput, *accPassInput;
    appInput = widgets->appInput;
    userInput = widgets->addUserInput;
    passInput = widgets->addPassInput;
    accPassInput = widgets->accPassInput;

    //app entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(appInput), "Application");
    gtk_grid_attach_next_to(GTK_GRID(grid), appInput, appLabel, GTK_POS_RIGHT, 1, 1);

    //user entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(userInput), "Username"); 
    gtk_grid_attach_next_to(GTK_GRID(grid), userInput, userLabel, GTK_POS_RIGHT, 1, 1);

    //password entry
    gtk_entry_set_placeholder_text(GTK_ENTRY(passInput), "Password");
    //gtk_entry_set_visibility(GTK_ENTRY(passInput), FALSE);
    //gtk_entry_set_input_purpose(GTK_ENTRY(passInput), GTK_INPUT_PURPOSE_PASSWORD);
    //gtk_entry_set_invisible_char(GTK_ENTRY(passInput), '*');
    gtk_grid_attach_next_to(GTK_GRID(grid), passInput, passLabel, GTK_POS_RIGHT, 1, 1);

    //account password 
    gtk_entry_set_placeholder_text(GTK_ENTRY(accPassInput), "Account Password (Needed for encryption)");
    gtk_entry_set_visibility(GTK_ENTRY(accPassInput), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(accPassInput), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_invisible_char(GTK_ENTRY(accPassInput), '*');
    gtk_grid_attach_next_to(GTK_GRID(grid), accPassInput, accPassLabel, GTK_POS_RIGHT, 1, 1);

    //populate form data to pass
    AddForm *form = g_new(AddForm, 1);

    form->appInput = appInput;
    form->login.userInput = userInput;
    form->login.passInput = passInput;
    form->login.widgets = widgets; 
    form->accPassInput = accPassInput;

    //submit button
    GtkWidget *submitButton = gtk_button_new_with_label("Add To Vault");
    gtk_grid_attach(GTK_GRID(grid), submitButton, 1, 5, 1, 1);
    g_signal_connect(submitButton, "clicked", G_CALLBACK(addToFile), form);

    //main menu button
    GtkWidget *menuButton = gtk_button_new_with_label("Return to Menu");
    g_object_set_data(G_OBJECT(menuButton), "page", "menu");
    gtk_grid_attach(GTK_GRID(grid), menuButton, 1, 6, 1, 1);
    g_signal_connect(menuButton, "clicked", G_CALLBACK(handle_page_switch), widgets);
}

void addToFile(GtkWidget *button, gpointer *userData) {

    //extracting data
    AddForm *form = (AddForm *)userData;
    const char *application = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->appInput)));
    const char *username = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->login.userInput)));
    const char *password = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->login.passInput)));
    const char *accPass = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(form->accPassInput)));
    AppWidgets *widgets = form->login.widgets;
    
    if (verifyPassword(accPass)) {
	//gets master storage path
	char *storagePath = getMasterStoragePath();

	//create file object
	FILE *storageFile = fopen(storagePath, "a");

	//allocate enough memory to store string
	char *storedString = malloc(strlen(application) + strlen(username) + strlen(password) + 1);

	//copy and concatenate to add everything
	strcpy(storedString, application);
	strcat(storedString, "|");
	strcat(storedString, username);
	strcat(storedString, "|");
	strcat(storedString, password);

	unsigned char *encryptedAesKey = retrieveDecryptedAESKey(accPass); 

	//encrypting before storing
	char *encryptedText = encryptText(storedString, encryptedAesKey);
	fprintf(storageFile, "%s\n", encryptedText);
	free(encryptedText);

	gtk_editable_set_text(GTK_EDITABLE(form->appInput), "");
	gtk_editable_set_text(GTK_EDITABLE(form->login.userInput), "");
	gtk_editable_set_text(GTK_EDITABLE(form->login.passInput), "");

	addSuccessfulNotification(form->login.widgets, 1);
	free(encryptedAesKey);
	free(storagePath);
	free(storedString);
	fclose(storageFile);
    } else {
	addSuccessfulNotification(form->login.widgets, 0);
    }


    return;
}

char *getMasterStoragePath() {
    char *home = getHomeEnv();
    size_t len = strlen(home) + strlen("/.config/passwordManager/storage.db") + 1;
    char *storagePath = malloc(len);
    if (storagePath != NULL) {
	snprintf(storagePath, len, "%s/.config/passwordManager/storage.db", home);
    }
    return storagePath;
}

// shows successful label for 3 seconds upon adding entry to storage
void addSuccessfulNotification(AppWidgets *widgets, int success) {
    GtkWidget *grid = widgets->add_page;
    GtkWidget *notificationLabel;

    if (success) {
	notificationLabel= gtk_label_new("Successfully added record to storage");
    } else {
	notificationLabel = gtk_label_new("Failed to add record, incorrect password");
    }

    gtk_grid_attach(GTK_GRID(grid), notificationLabel, 1, 7, 1, 1);
    g_timeout_add_seconds(3, (GSourceFunc)remove_notification_label, notificationLabel);
}

//function to safely remove success label
void remove_notification_label(gpointer data) {
    gtk_widget_unparent(GTK_WIDGET(data));
}

void setup_list_page(AppWidgets *widgets) {
    GtkWidget *box = widgets->list_page;
    GtkWidget *passEntry = widgets->listPassInput;

    GtkWidget *passLabel = gtk_label_new("Enter Password to View");
    gtk_box_append(GTK_BOX(box), passLabel);
    gtk_box_append(GTK_BOX(box), passEntry);

    gtk_entry_set_placeholder_text(GTK_ENTRY(passEntry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(passEntry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(passEntry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_invisible_char(GTK_ENTRY(passEntry), '*');
}

