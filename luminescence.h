#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;     // GtkWindow
    GtkWidget *status_bar; // GtkBox
    GtkWidget *web_view;   // WebKitWebView
    void (*focus)(void);
    void (*exec)(const char*, const char*);
    // bind: modifier, key, command, argument
    void* (*bind)(guint, guint, const char*, const char*);
    void (*unbind)(void*);
} Lumi;

typedef struct {
    const char *name;
    void (*exec)(const char*);
    const char *help;
} Command;

#endif // LUMINESCENCE_H
