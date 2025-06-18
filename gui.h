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

void activate(GtkApplication *app, gpointer user_data);

//Gui Components
void initWarning(GtkWidget *window);
void loginScreen(GtkWidget *window);
void validateLogin(GtkWidget *button, gpointer user_data);

#endif // GUI_H
