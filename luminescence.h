#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;     // GtkWindow
    GtkWidget *status_bar; // GtkBox
    GtkWidget *web_view;   // WebKitWebView
    void (*execv)(int, char**);
    void (*execl)(const char*, ...);
} Lumi;

typedef struct {
    const char *name;
    void (*exec)(int, char**);
    const char *help;
} Command;

#define lumi_exec(cmd, ...)    (*lumi->execl)((cmd), __VA_ARGS__)
#define lumi_execv(argc, argv) (*lumi->execv)((argc), (argv))

#endif // LUMINESCENCE_H
