#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *web_view;
GtkWidget *entry;

void go(){
    const gchar *raw_uri = gtk_entry_get_text(GTK_ENTRY(entry));
    gchar *uri = (gchar*) malloc(strlen(raw_uri)+8);
    uri[0] = '\0';
    if(!strstr(raw_uri, "://"))
        strcat(uri, "http://");
    strcat(uri, raw_uri);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), uri);
    free(uri);
    gtk_widget_hide(entry);
}

int key_callback(GdkEventKey *e){
    if(e->keyval == GDK_KEY_Escape){
        gtk_widget_hide(entry);
        return FOCUS_RELEASE | EVENT_PROPAGATE;
    }
    else if(gtk_widget_get_visible(entry))
        return FOCUS_GRAB | EVENT_PROPAGATE;
    else if(e->keyval == GDK_KEY_o){
        const gchar *uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(web_view));
        gtk_entry_set_text(GTK_ENTRY(entry), uri ? uri : "");
        gtk_widget_show(entry);
        gtk_widget_grab_focus(entry);
        return FOCUS_GRAB | EVENT_STOP;
    }
    return EVENT_PROPAGATE;
}

void init(Lumi *lumi){
    web_view = lumi->web_view;
    entry = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(entry), FALSE);
    g_signal_connect(entry, "activate", G_CALLBACK(go), NULL);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), entry, TRUE, TRUE, 0);
}
