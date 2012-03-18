#include <gtk/gtk.h>
#include <webkit/webkit.h>

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *web_view = webkit_web_view_new();
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), "file:///");

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), web_view);
    gtk_box_pack_start(GTK_BOX(layout), scrolled_window, TRUE, TRUE, 0);

    GtkWidget *status = gtk_label_new("Hello world!");
    gtk_misc_set_alignment(GTK_MISC(status), 0, 0.5);
    gtk_box_pack_start(GTK_BOX(layout), status, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
