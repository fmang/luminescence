#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *window;     // GtkWindow
    GtkWidget *status_bar; // GtkBox
    GtkWidget *web_view;   // WebKitWebView
} Lumi;

// Events

#define EVENT_PROPAGATE 0
#define EVENT_STOP      1

#define FOCUS_RELEASE   0
#define FOCUS_GRAB      2

// Returns an OR'd combination of the above flags.
typedef int KeyCallback(GdkEventKey*);

// Options

#define NO_ARGUMENT       0
#define OPTIONAL_ARGUMENT 1
#define REQUIRED_ARGUMENT 2

typedef struct {
    const char *name;
    int argument;
} Option;

#endif // LUMINESCENCE_H
