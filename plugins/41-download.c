#include <luminescence.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

const char *name = "Download";

typedef struct {
    WebKitDownload *download;
    GtkWidget *progress_bar;
} Download;

Lumi *lumi;

void update_progress(GObject *obj, GParamSpec *specs, Download *dw){
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(dw->progress_bar), webkit_download_get_progress(dw->download));
}

void clean(GObject *obj, GParamSpec *specs, Download *dw){
    switch(webkit_download_get_status(dw->download)){
        case WEBKIT_DOWNLOAD_STATUS_CANCELLED:
        case WEBKIT_DOWNLOAD_STATUS_FINISHED:
            gtk_widget_destroy(dw->progress_bar);
            g_object_unref(dw->download);
            free(dw);
            break;
        default: break;
    }
}

void register_download(WebKitDownload *d){
    Download *dw = (Download*) malloc(sizeof(Download));
    dw->download = d;
    dw->progress_bar = gtk_progress_bar_new();
    gtk_widget_set_tooltip_text(dw->progress_bar, webkit_download_get_destination_uri(d));
    g_signal_connect(dw->download, "notify::progress", G_CALLBACK(update_progress), dw);
    g_signal_connect(dw->download, "notify::status", G_CALLBACK(clean), dw);
    gtk_box_pack_start(GTK_BOX(lumi->status_bar), dw->progress_bar, FALSE, FALSE, 3);
    gtk_widget_show(dw->progress_bar);
}

char* unused_filename(const char *f){
    char *fn = (char*) malloc(strlen(f) + 4);
    strcpy(fn, f);
    char *n = fn + strlen(f);
    int i = 0;
    for(; i<100 && access(fn+7, F_OK)==0; i++){
        *n = '.';
        sprintf(n+1, "%d", i);
    }
    return fn;
}

void download_c(int argc, char **argv){
    if(argc != 3) return;
    WebKitNetworkRequest *request = webkit_network_request_new(argv[1]);
    WebKitDownload *dw = webkit_download_new(request);
    char *dst = unused_filename(argv[2]);
    webkit_download_set_destination_uri(dw, dst);
    register_download(dw);
    webkit_download_start(dw);
}

Command commands[] = {
    { "download", download_c },
    { 0 } };

#define DESTDIR "file:///tmp/dw/"

gboolean download_requested(WebKitWebView *view, WebKitDownload *dw){
    mkdir(DESTDIR+7, 0777); // 7 = strlen("file://")
    const gchar *name = webkit_download_get_suggested_filename(dw);
    char *fn = (char*) malloc(strlen(name) + strlen(DESTDIR) + 1);
    strcpy(fn, DESTDIR);
    strcat(fn, name);
    char *dst = unused_filename(fn);
    free(fn);
    webkit_download_set_destination_uri(dw, dst);
    register_download(dw);
    return TRUE;
}

void init(){
    g_signal_connect(lumi->web_view, "download_requested", G_CALLBACK(download_requested), NULL);
}
