#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "passwordManager.h"
#include "cliFunctions.h"


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
} AppWidgets;

typedef struct {
    GtkWidget *userInput;
    GtkWidget *passInput;
    AppWidgets *widgets;
} LoginForm;

typedef struct {
    GtkWidget *appInput;
    GtkWidget *accPassInput;
    LoginForm login;
} AddForm;

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
void addSuccessfulNotification(AppWidgets *widgets);
void remove_success_label(gpointer data);

#endif // GUI_H
