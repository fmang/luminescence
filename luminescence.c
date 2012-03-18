#include <gtk/gtk.h>

int main(int argc, char **argv){
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(win, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show(win);
    gtk_main();
    return 0;
}
