#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "passwordManager.h"
#include "cliFunctions.h"

typedef struct {
    GtkWidget *userInput;
    GtkWidget *passInput;
} LoginForm;

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

    // Widget for forms
    GtkWidget *userInput;
    GtkWidget *passInput;
} AppWidgets;

//entry function
void activate(GtkApplication *app, gpointer user_data);

//Page setups
void setup_warning_page(AppWidgets *widgets);
void setup_menu_page(AppWidgets *widgets);
void setup_login_page(AppWidgets *widgets);

//Utility functions
void switch_page(AppWidgets *widgets, const char *page_name);
void validateLogin(GtkWidget *button, gpointer user_data);

#endif // GUI_H
