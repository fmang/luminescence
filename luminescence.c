#include <gtk/gtk.h>
#include <webkit/webkit.h>

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(win, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *web = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(win), web);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web), "file:///");

    gtk_widget_show_all(win);
    gtk_main();

    return 0;
}
