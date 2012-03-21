#include <gtk/gtk.h>
#include <webkit/webkit.h>

struct {
    GtkWidget *web_view;
    GtkWidget *address_bar;
} ui;

void go(){
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui.web_view), gtk_entry_get_text(GTK_ENTRY(ui.address_bar)));
}

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    // Window

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Web view

    ui.web_view = webkit_web_view_new();
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui.web_view), "file:///");

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), ui.web_view);
    gtk_box_pack_start(GTK_BOX(layout), scrolled_window, TRUE, TRUE, 0);

    // Status

    GtkWidget *status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(layout), status_bar, FALSE, FALSE, 0);

    ui.address_bar = gtk_entry_new();
    gtk_entry_set_has_frame(GTK_ENTRY(ui.address_bar), FALSE);
    g_signal_connect(ui.address_bar, "activate", G_CALLBACK(go), NULL);
    gtk_box_pack_start(GTK_BOX(status_bar), ui.address_bar, TRUE, TRUE, 0);

    // Show

    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
