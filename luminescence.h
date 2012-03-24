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

#define EVENT_PROPAGATE 0
#define EVENT_STOP      1

// Returns either of the above constants.
typedef int KeyCallback(GdkEventKey*);

#endif // LUMINESCENCE_H
