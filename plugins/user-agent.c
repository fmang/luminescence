#include <luminescence.h>

const char *name = "User Agent";

Lumi *lumi;

void set_user_agent(int argc, char **argv){
    g_object_set(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(lumi->web_view)),
        "user-agent", (argc>1 ? argv[1] : 0), NULL);
}

Command commands[] = {
    { "user-agent", set_user_agent },
    { 0 } };
