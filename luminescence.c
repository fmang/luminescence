#include <gtk/gtk.h>
#include <webkit/webkit.h>

void go(GtkEntry *entry, WebKitWebView *view){
    webkit_web_view_load_uri(view, gtk_entry_get_text(entry));
}

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    // Window

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Web view

    GtkWidget *web_view = webkit_web_view_new();
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), "file:///");

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), web_view);
    gtk_box_pack_start(GTK_BOX(layout), scrolled_window, TRUE, TRUE, 0);

    // Status

    GtkWidget *status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(layout), status_bar, FALSE, FALSE, 0);

    GtkWidget *address_bar = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(address_bar), FALSE);
    g_signal_connect(address_bar, "activate", G_CALLBACK(go), web_view);
    gtk_box_pack_start(GTK_BOX(status_bar), address_bar, TRUE, TRUE, 0);

    // Show

    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
