#include <luminescence.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *name = "Window title";

Lumi *lumi;
char *suffix;

void set_title(WebKitWebView *web_view){
    const char *title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(lumi->web_view));
    if(title){
        char *window_title = (char*) malloc(strlen(title)+strlen(suffix)+4);
        sprintf(window_title, "%s - %s", title, suffix);
        gtk_window_set_title(GTK_WINDOW(lumi->window), window_title);
        free(window_title);
    }
    else
        gtk_window_set_title(GTK_WINDOW(lumi->window), suffix);
}

void init(){
    suffix = (char*) malloc(32);
    sprintf(suffix, "Luminescence <%d>", getpid());
    set_title(WEBKIT_WEB_VIEW(lumi->web_view));
    g_signal_connect(lumi->web_view, "notify::title", G_CALLBACK(set_title), NULL);
}
