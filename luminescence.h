#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;        // GtkWindow
    GtkWidget *status_bar;    // GtkBox
    GtkWidget *web_view;      // WebKitWebView
    GtkWidget *address_label; // GtkLabel
    GtkWidget *address_entry; // GtkEntry
    GtkWidget *scripts_label; // GtkLabel
    GtkWidget *insert_label;  // GtkLabel
} Lumi;

#endif // LUMINESCENCE_H
