#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;     // GtkWindow
    GtkWidget *status_bar; // GtkBox
    GtkWidget *web_view;   // WebKitWebView
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

#define lumi_focus() (*lumi->exec)("focus", 0)
#define lumi_leave() (*lumi->exec)("leave", 0)

#endif // LUMINESCENCE_H
