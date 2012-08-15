#include <luminescence.h>
#include <string.h>

const char *name = "Navigation";

Lumi *lumi;

void go_back(){ webkit_web_view_go_back(WEBKIT_WEB_VIEW(lumi->web_view)); }
void go_forward(){ webkit_web_view_go_forward(WEBKIT_WEB_VIEW(lumi->web_view)); }

void reload(){ webkit_web_view_reload(WEBKIT_WEB_VIEW(lumi->web_view)); }
void reload_nocache(){ webkit_web_view_reload_bypass_cache(WEBKIT_WEB_VIEW(lumi->web_view)); }

void enable_page_cache(int argc, char **argv){
    gboolean enable;
    if(argc <= 1) enable = TRUE;
    else if(strcasecmp("on", argv[1]) == 0) enable = TRUE;
    else if(strcasecmp("off", argv[1]) == 0) enable = FALSE;
    else return;
    g_object_set(webkit_web_view_get_settings(
        WEBKIT_WEB_VIEW(lumi->web_view)),
        "enable-page-cache", enable, NULL);
}

Command commands[] = {
    { "back", go_back },
    { "forward", go_forward },
    { "reload", reload },
    { "Reload", reload_nocache, "Bypass the cache." },
    { "page-cache", enable_page_cache, "Enable WebKit's page cache." },
    { 0 } };
