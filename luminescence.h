#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;     // GtkWindow
    GtkWidget *status_bar; // GtkBox
    GtkWidget *web_view;   // WebKitWebView
} Lumi;

typedef struct {
    const char *name;
    void (*exec)(const char*);
    const char *help;
} Command;

#endif // LUMINESCENCE_H
