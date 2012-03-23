#ifndef LUMINESCENCE_H
#define LUMINESCENCE_H

#include <gtk/gtk.h>
#include <webkit/webkit.h>

typedef struct {
    GtkWidget *web_view;
    GtkWidget *address_label;
    GtkWidget *address_entry;
    GtkWidget *scripts_label;
    GtkWidget *insert_label;
} Lumi;

#endif // LUMINESCENCE_H
