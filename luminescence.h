#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;        // GtkWindow
    GtkWidget *status_bar;    // GtkBox
    GtkWidget *web_view;      // WebKitWebView
    GtkWidget *scripts_label; // GtkLabel
} Lumi;

#define EVENT_PROPAGATE 0
#define EVENT_STOP      1

#define FOCUS_RELEASE   0
#define FOCUS_GRAB      2

// Returns an OR'd combination of the above flags.
typedef int KeyCallback(GdkEventKey*);

#endif // LUMINESCENCE_H
