#include <luminescence.h>

const char *name = "URI viewer";

const char *description = "Press u to display the current page's or the hovered link's URI.";

GtkWidget *uri_label;

void update_uri(GtkWidget *view){
    if(webkit_web_view_get_load_status(WEBKIT_WEB_VIEW(view)) == WEBKIT_LOAD_COMMITTED)
        gtk_label_set_text(GTK_LABEL(uri_label), webkit_web_view_get_uri(WEBKIT_WEB_VIEW(view)));
}

void link_hovered(WebKitWebView *view, gchar *title, gchar *uri){
    gtk_label_set_text(GTK_LABEL(uri_label), uri ? uri : webkit_web_view_get_uri(view));
}

int key_callback(GdkEventKey *e){
    if(gtk_widget_get_visible(uri_label) && e->keyval == GDK_KEY_y){
        gtk_clipboard_set_text(gtk_clipboard_get(GDK_SELECTION_PRIMARY), gtk_label_get_text(GTK_LABEL(uri_label)), -1);
        gtk_widget_hide(uri_label);
        return FOCUS_RELEASE | EVENT_STOP;
    }
    else if(e->keyval == GDK_KEY_u){
        gtk_widget_show(uri_label);
        return FOCUS_GRAB | EVENT_STOP;
    }
    else if(gtk_widget_get_visible(uri_label)){
        gtk_widget_hide(uri_label);
        return FOCUS_RELEASE | EVENT_PROPAGATE;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    uri_label = gtk_label_new("");
    gtk_label_set_selectable(GTK_LABEL(uri_label), TRUE);
    gtk_misc_set_alignment(GTK_MISC(uri_label), 0, 0.5);
    g_signal_connect(lumi->web_view, "notify::load-status", G_CALLBACK(update_uri), NULL);
    g_signal_connect(lumi->web_view, "hovering-over-link", G_CALLBACK(link_hovered), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), uri_label, TRUE, TRUE, 0);
}
