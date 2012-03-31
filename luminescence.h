#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;     // GtkWindow
    GtkWidget *status_bar; // GtkBox
    GtkWidget *web_view;   // WebKitWebView

    void (*exec)(const char*, const char*);
    void* (*bind)(guint, guint, const char*, const char*);
    void (*unbind)(void*);
} Lumi;

typedef struct {
    const char *name;
    void (*exec)(const char*);
    const char *help;
} Command;

#define lumi_exec(cmd, arg) (*lumi->exec)((cmd), (arg))
#define lumi_bind(key, cmd, arg) (*lumi->bind)(0, (key), (cmd), (arg))
#define lumi_bind_mod(mod, key, cmd, arg) (*lumi->bind)((mod), (key), (cmd), (arg))
#define lumi_unbind(b) (*lumi->unbind)((b))

#define lumi_focus() lumi_exec("focus", 0)
#define lumi_leave() lumi_exec("leave", 0)

#endif // LUMINESCENCE_H
