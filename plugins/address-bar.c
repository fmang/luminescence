#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *web_view;

GtkWidget *address_label;
GtkWidget *address_entry;

void update_address_label(){
    if(webkit_web_view_get_load_status(WEBKIT_WEB_VIEW(web_view)) == WEBKIT_LOAD_COMMITTED)
        gtk_label_set_text(GTK_LABEL(address_label), webkit_web_view_get_uri(WEBKIT_WEB_VIEW(web_view)));
}

void display_hovered_uri(WebKitWebView *view, gchar *title, gchar *uri){
    gtk_label_set_text(GTK_LABEL(address_label),
        uri ? uri : webkit_web_view_get_uri(view));
}

void go(){
    const gchar *raw_uri = gtk_entry_get_text(GTK_ENTRY(address_entry));
    gchar *uri = (gchar*) malloc(strlen(raw_uri)+8);
    uri[0] = '\0';
    if(!strstr(raw_uri, "://"))
        strcat(uri, "http://");
    strcat(uri, raw_uri);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), uri);
    free(uri);
    gtk_widget_hide(address_entry);
    gtk_widget_show(address_label);
}

int key_callback(GdkEventKey *e){
    if(e->keyval == GDK_KEY_Escape){
        gtk_widget_hide(address_entry);
        gtk_widget_show(address_label);
        return FOCUS_RELEASE | EVENT_PROPAGATE;
    }
    else if(gtk_widget_get_visible(address_entry))
        return FOCUS_GRAB | EVENT_PROPAGATE;
    else if(e->keyval == GDK_KEY_u){
        // Display the entry
        const gchar *uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(web_view));
        gtk_entry_set_text(GTK_ENTRY(address_entry), uri ? uri : "");
        gtk_widget_hide(address_label);
        gtk_widget_show(address_entry);
        gtk_widget_grab_focus(address_entry);
        return FOCUS_GRAB | EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    web_view = lumi->web_view;

    address_label = gtk_label_new("");
    gtk_label_set_selectable(GTK_LABEL(address_label), TRUE);
    gtk_misc_set_alignment(GTK_MISC(address_label), 0, 0.5);
    gtk_misc_set_padding(GTK_MISC(address_label), 2, 2);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), address_label, TRUE, TRUE, 0);
    gtk_widget_show(address_label);

    address_entry = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(address_entry), FALSE);
    g_signal_connect(address_entry, "activate", G_CALLBACK(go), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), address_entry, TRUE, TRUE, 0);

    g_signal_connect(web_view, "notify::load-status", G_CALLBACK(update_address_label), NULL);
    g_signal_connect(web_view, "hovering-over-link", G_CALLBACK(display_hovered_uri), NULL);
}
