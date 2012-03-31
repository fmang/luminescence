#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "Window Title";

Lumi *lumi;

void set_title(WebKitWebView *web_view){
    const char *title = webkit_web_view_get_title(WEBKIT_WEB_VIEW(lumi->web_view));
    if(title){
        char *window_title = (char*) malloc(strlen(title)+16);
        strcpy(window_title, title);
        strcat(window_title, " - Luminescence");
        gtk_window_set_title(GTK_WINDOW(lumi->window), window_title);
        free(window_title);
    }
    else
        gtk_window_set_title(GTK_WINDOW(lumi->window), "Luminescence");
}

void init(){
    set_title(WEBKIT_WEB_VIEW(lumi->web_view));
    g_signal_connect(lumi->web_view, "notify::title", G_CALLBACK(set_title), NULL);
}
