#include <gtk/gtk.h>
#include <webkit/webkit.h>

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *web_view = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(window), web_view);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), "file:///");

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
