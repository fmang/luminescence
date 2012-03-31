#include <luminescence.h>

const char *name = "Reload";

Lumi *lumi;

void reload(){
    webkit_web_view_reload(WEBKIT_WEB_VIEW(lumi->web_view));
}

void reload_nocache(){
    webkit_web_view_reload_bypass_cache(WEBKIT_WEB_VIEW(lumi->web_view));
}

Command commands[] = {
    { "reload", reload },
    { "Reload", reload_nocache, "Bypass the cache." },
    { 0 } };
