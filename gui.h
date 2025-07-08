#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "passwordManager.h"
#include "cliFunctions.h"


//info object to hold all widgets as part of stack
typedef struct {
    GtkWidget *stack;

    //Pages
    GtkWidget *warning_page;
    GtkWidget *login_page;
    GtkWidget *menu_page;
    GtkWidget *add_page;
    GtkWidget *delete_page;
    GtkWidget *list_page;
    GtkWidget *generate_page;

    // Widget for login form
    GtkWidget *userInput;
    GtkWidget *passInput;

    //Widgets for adding form
    GtkWidget *appInput;
    GtkWidget *addUserInput;
    GtkWidget *addPassInput;
    GtkWidget *accPassInput;

    //Widgets for list_page
    GtkWidget *listPassInput;
} AppWidgets;

//info object for logging in 
typedef struct {
    GtkWidget *userInput;
    GtkWidget *passInput;
    AppWidgets *widgets;
} LoginForm;

//info object for adding accoutns
typedef struct {
    GtkWidget *appInput;
    GtkWidget *accPassInput;
    LoginForm login;
} AddForm;

typedef struct {
    GtkWidget *passInput;
} ListForm;

//entry function
void activate(GtkApplication *app, gpointer user_data);

//Page setups
void setup_warning_page(AppWidgets *widgets);
void setup_menu_page(AppWidgets *widgets);
void setup_login_page(AppWidgets *widgets);
void setup_add_page(AppWidgets *widgets);
void setup_list_page(AppWidgets *widgets);

//Core functionality functions
void switch_page(AppWidgets *widgets, const char *page_name);
void handle_page_switch(GtkButton *button, gpointer user_data);

//Login functionality
void validateLogin(GtkWidget *button, gpointer user_data);

//Password storing functionality
void addToFile(GtkWidget *button, gpointer *userData);
char *getMasterStoragePath();
void addSuccessfulNotification(AppWidgets *widgets, int success);
void remove_notification_label(gpointer data);

//Listing accounts functionality
void list_login(GtkWidget *button, gpointer passData);

#endif // GUI_H
