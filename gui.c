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
    
    //Generate page entries
    widgets->generatePassInput = gtk_entry_new();

    //Add to stack
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->warning_page, "warning");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->menu_page, "menu");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->login_page, "login");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->add_page, "add");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->list_page, "list");
    gtk_stack_add_named(GTK_STACK(widgets->stack), widgets->generate_page, "generate");

    //Setup pages
    setup_warning_page(widgets);
    setup_menu_page(widgets);
    setup_login_page(widgets);
    setup_add_page(widgets);
    setup_list_page(widgets);
    setup_generate_page(widgets);
    
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
        if (strcmp(page_name, "list") == 0) {
            GtkWidget *child = gtk_widget_get_first_child(widgets->list_page);
            while (child) {
                GtkWidget *next = gtk_widget_get_next_sibling(child);
                gtk_box_remove(GTK_BOX(widgets->list_page), child);
                child = next;
            }
            widgets->listPassInput = gtk_entry_new();
            setup_list_page(widgets);
        }
	switch_page(widgets, page_name);
    } else{
	g_warning("No page found with that name!");
    }
}
//clears list page widgets to update 
void clear_list_entries_only(AppWidgets *widgets) {
    GtkWidget *box = widgets->list_page;
    GtkWidget *child = gtk_widget_get_first_child(box);
    
    int skip_count = 0;
    while (child && skip_count < 3) {
        child = gtk_widget_get_next_sibling(child);
        skip_count++;
    }
    
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(box), child);
        child = next;
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
    GtkWidget *list_btn = gtk_button_new_with_label("List Passwords");
    GtkWidget *gen_btn = gtk_button_new_with_label("Generate Pssword");

    //Add to box
    gtk_box_append(GTK_BOX(menu), add_btn);
    gtk_box_append(GTK_BOX(menu), list_btn);
    gtk_box_append(GTK_BOX(menu), gen_btn);

    //Navigation hooks
    g_object_set_data(G_OBJECT(add_btn), "page", "add");
    g_signal_connect(add_btn, "clicked", G_CALLBACK(handle_page_switch), widgets);

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
	char *storedString = malloc(strlen(application) + strlen(username) + strlen(password) + 3);

	//copy and concatenate to add everything
	strcpy(storedString, application);
	strcat(storedString, "|");
	strcat(storedString, username);
	strcat(storedString, "|");
	strcat(storedString, password);

	unsigned char *aesKey = retrieveDecryptedAESKey(accPass);
	if (!aesKey) {
	    addSuccessfulNotification(form->login.widgets, 0);
	    free(storagePath);
	    free(storedString);
	    fclose(storageFile);
	    return;
	}

	//encrypting before storing
	char *encryptedText = encryptText(storedString, (char *)aesKey);
	if (!encryptedText) {
	    addSuccessfulNotification(form->login.widgets, 0);
	    free(aesKey);
	    free(storagePath);
	    free(storedString);
	    fclose(storageFile);
	    return;
	}
	fprintf(storageFile, "%s\n", encryptedText);
	free(encryptedText);

	gtk_editable_set_text(GTK_EDITABLE(form->appInput), "");
	gtk_editable_set_text(GTK_EDITABLE(form->login.userInput), "");
	gtk_editable_set_text(GTK_EDITABLE(form->login.passInput), "");

	addSuccessfulNotification(form->login.widgets, 1);
	free(aesKey);
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
    GtkWidget *passEntry = widgets->listPassInput; // loads password entry

    //creating password label
    GtkWidget *passLabel = gtk_label_new("Enter Password to View");
    gtk_box_append(GTK_BOX(box), passLabel);
    gtk_box_append(GTK_BOX(box), passEntry);

    //important settings 
    gtk_entry_set_placeholder_text(GTK_ENTRY(passEntry), "Password");
    gtk_entry_set_visibility(GTK_ENTRY(passEntry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(passEntry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_invisible_char(GTK_ENTRY(passEntry), '*');

    ListForm *entryData = g_new(ListForm, 1);
    entryData->passInput = passEntry;
    entryData->passLabel = passLabel;
    entryData->box = box;
    entryData->widgets = widgets;

    //create button
    GtkWidget *submitButton = gtk_button_new_with_label("See Accounts");
    gtk_box_append(GTK_BOX(box), submitButton);
    g_signal_connect(submitButton, "clicked", G_CALLBACK(list_login), entryData);

    //return to menu
    GtkWidget *menuButton = gtk_button_new_with_label("Return to Menu");
    g_object_set_data(G_OBJECT(menuButton), "page", "menu");
    gtk_box_append(GTK_BOX(box), menuButton);
    g_signal_connect(menuButton, "clicked", G_CALLBACK(handle_page_switch), widgets);
}

void list_login(GtkWidget *button, gpointer passData) {
    ListForm *data = (ListForm *)passData;
    GtkWidget *passInput = data->passInput;
    GtkWidget *passLabel = data->passLabel;
    GtkWidget *box = data->box;
    const char *accPassTemp = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(data->passInput)));
    
    // Make a copy of the password to avoid memory issues
    char *accPass = strdup(accPassTemp);

    if (verifyPassword(accPass)) {
	//remove verification widgets
	gtk_box_remove(GTK_BOX(box), passLabel);
	gtk_box_remove(GTK_BOX(box), button);
	gtk_box_remove(GTK_BOX(box), passInput);

	//Load entries properly
	unsigned char *aesKey = retrieveDecryptedAESKey(accPass);
	if (!aesKey) {
	    GtkWidget *errorLabel = gtk_label_new("Failed to decrypt AES key - wrong password?");
	    gtk_box_append(GTK_BOX(box), errorLabel);
	    free(accPass);
	    return;
	}
	
	char fullpath[512];
	snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/storage.db", getHomeEnv());
	FILE *storageFile = fopen(fullpath, "r");

	if (storageFile) {
	    char buffer[512];
	    while (fgets(buffer, sizeof(buffer), storageFile)) {
		// Remove newline character properly
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len-1] == '\n') {
		    buffer[len - 1] = '\0';
		}

		// Skip empty lines
		if (strlen(buffer) == 0) continue;

		//decyphers line
		char *decrypted = decryptText(buffer, aesKey);

		//creating entry box
		GtkWidget *entryBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
		char *token = strtok(decrypted, "|");
		int count = 1;

		if (decrypted) {
		    while (token != NULL) {
			GtkWidget *nextLabel = NULL;
			switch (count) {
			    case 1: { //app case
				char *formatted = g_strdup_printf("Application: %s", token);
				nextLabel = gtk_label_new(formatted);
				g_free(formatted);
				break;
			    }
			    case 2: { //username case
				char *formatted = g_strdup_printf("Username: %s", token);
				nextLabel = gtk_label_new(formatted);
				g_free(formatted);
				break;
			    }
			    case 3: { //password case
				char *formatted = g_strdup_printf("Password: %s", token);
				nextLabel = gtk_label_new(formatted);
				g_free(formatted);
				break;
			    }
			}
			if (nextLabel) {
			    gtk_box_append(GTK_BOX(entryBox), nextLabel);
			}

			token = strtok(NULL, "|");
			count++;
		    }
		    GtkWidget *deleteLabel = gtk_button_new_with_label("Delete");
		    char *encryptedLine = strdup(buffer);
		    g_object_set_data_full(G_OBJECT(deleteLabel), "encrypted_line", encryptedLine, g_free);
		    g_signal_connect(deleteLabel, "clicked", G_CALLBACK(delete_entry), entryBox);
		    gtk_box_append(GTK_BOX(entryBox), deleteLabel);

		    free(decrypted);
		}
		gtk_box_append(GTK_BOX(box), entryBox);
	    }
	    fclose(storageFile);
	}
	
	free(aesKey);
    } else {
	GtkWidget *failLabel = gtk_label_new("Incorect password try again");
	gtk_box_append(GTK_BOX(box), failLabel);
	g_timeout_add_seconds(3, (GSourceFunc)remove_notification_label, failLabel);
    }
    
    free(accPass);
    return;
}

void delete_entry(GtkWidget *button, gpointer user_data) {
    GtkWidget *entryBox = (GtkWidget *)user_data;
    GtkWidget *parentBox = gtk_widget_get_parent(entryBox); 

    char *encryptedLine = g_object_get_data(G_OBJECT(button), "encrypted_line");

    if (encryptedLine) {
	char fullpath[512];
	snprintf(fullpath, sizeof(fullpath), "%s/.config/passwordManager/storage.db", getHomeEnv());

	FILE *originalFile = fopen(fullpath, "r");
	char tempPath[520];
	snprintf(tempPath, sizeof(tempPath), "%s.temp", fullpath);
	FILE *tempFile = fopen(tempPath, "w");

	if (originalFile && tempFile) {
	    char buffer[512];
	    while (fgets(buffer, sizeof(buffer), originalFile)) {
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strcmp(buffer, encryptedLine) != 0) {
		    fprintf(tempFile, "%s\n", buffer);
		}
	    }

	    fclose(originalFile);
	    fclose(tempFile);

	    remove(fullpath);
	    rename(tempPath, fullpath);
	}
    }

    gtk_box_remove(GTK_BOX(parentBox), entryBox);
}

void setup_generate_page(AppWidgets *widgets) {
    GtkWidget *generate = widgets->generate_page;
    GtkWidget *centerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(centerBox, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(centerBox, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(generate), centerBox);

    //title
    GtkWidget *generateLabel = gtk_label_new("Password Generator");
    gtk_widget_set_halign(generateLabel, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(centerBox), generateLabel);

    //generated password display
    GtkWidget *passwordEntry = widgets->generatePassInput;
    gtk_entry_set_placeholder_text(GTK_ENTRY(passwordEntry), "Generated Password");
    gtk_editable_set_editable(GTK_EDITABLE(passwordEntry), FALSE);
    gtk_widget_set_size_request(passwordEntry, 400, -1);
    gtk_box_append(GTK_BOX(centerBox), passwordEntry);
    
    //Length slider container
    GtkWidget *sliderBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(sliderBox, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(centerBox), sliderBox);

    //Length slider
    GtkAdjustment *adjustment = gtk_adjustment_new(10, 8, 32, 1, 1, 0);
    GtkWidget *lengthSlider = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, adjustment);
    gtk_widget_set_size_request(lengthSlider, 300, -1);
    gtk_scale_set_digits(GTK_SCALE(lengthSlider), 0);
    gtk_box_append(GTK_BOX(sliderBox), lengthSlider);

    //Length label
    GtkWidget *lengthLabel = gtk_label_new("10");
    gtk_box_append(GTK_BOX(sliderBox), lengthLabel);
    g_signal_connect(lengthSlider, "value-changed", G_CALLBACK(update_length_label), lengthLabel);
    
    //Populating required info 
    GenerateForm *generateData = g_new(GenerateForm, 1);
    generateData->passwordLabel = passwordEntry;
    generateData->lengthLabel = lengthLabel;

    //Generate button
    GtkWidget *generateButton = gtk_button_new_with_label("Generate Password");
    gtk_widget_set_halign(generateButton, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(centerBox), generateButton);
    g_signal_connect(generateButton, "clicked", G_CALLBACK(generate_password), generateData);

    //return to menu
    GtkWidget *menuButton = gtk_button_new_with_label("Return to Menu");
    gtk_widget_set_halign(menuButton, GTK_ALIGN_CENTER);
    g_object_set_data(G_OBJECT(menuButton), "page", "menu");
    gtk_box_append(GTK_BOX(centerBox), menuButton);
    g_signal_connect(menuButton, "clicked", G_CALLBACK(handle_page_switch), widgets);
}

//Function to update label
void update_length_label(GtkRange *range, gpointer user_data) {
    GtkWidget *label = (GtkWidget *)user_data;
    int value = (int)gtk_range_get_value(range);
    char text[10];
    snprintf(text, sizeof(text), "%d", value);
    gtk_label_set_text(GTK_LABEL(label), text);
}

void generate_password(GtkWidget *button, gpointer user_data) {
    const char *lower = "abcdefghijklmnopqrstuvwxyz";
    const char *upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *digits = "0123456789";
    const char *symbols = "!@#$%^&*()-_=+[]{};:,.<>?/|~";
    const char *all = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{};:,.<>?/|~";

    GenerateForm *generateData = (GenerateForm *)user_data;
    GtkWidget *passLabel = generateData->passwordLabel;
    GtkWidget *lengthLabel = generateData->lengthLabel;

    //extract length
    const char *lengthText = gtk_label_get_text(GTK_LABEL(lengthLabel));
    int length = atoi(lengthText);

    char *password = g_malloc0(length + 1);

    //ensure key characters
    password[0] = upper[g_random_int_range(0, strlen(upper))];
    password[1] = digits[g_random_int_range(0, strlen(digits))];
    password[2] = symbols[g_random_int_range(0, strlen(symbols))];

    for (int i = 3; i < length; i++) {
	password[i] = all[g_random_int_range(0, strlen(all))];
    }

    //shuffle password
    for (int i = length - 1; i > 0; i--) {
	int j = g_random_int_range(0, i+1);
	char tmp = password[i];
	password[i] = password[j];
	password[j] = tmp;
    }

    gtk_editable_set_editable(GTK_EDITABLE(generateData->passwordLabel), TRUE);
    gtk_editable_set_text(GTK_EDITABLE(generateData->passwordLabel), password);
    gtk_editable_set_editable(GTK_EDITABLE(generateData->passwordLabel), FALSE);
    g_free(password);

}
