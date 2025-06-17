#include "gui.h"

void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *box, *label;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Test 2");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 250);
    gtk_window_present(GTK_WINDOW(window));

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), box);

    label = gtk_label_new("Login");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), label);

}
