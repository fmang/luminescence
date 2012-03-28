#include <luminescence.h>
#include <stdlib.h>
#include <string.h>

const char *name = "User Agent";

char *user_agent = 0;
GtkWidget *web_view = 0;

void update_user_agent(){
    if(!web_view || !user_agent) return;
    g_object_set(webkit_web_view_get_settings(WEBKIT_WEB_VIEW(web_view)), "user-agent", user_agent, NULL);
}

void set_user_agent(const char *agent){
    if(user_agent) free(user_agent);
    user_agent = agent ? strdup(agent) : 0;
}

Option options[] = {
    { "user-agent", set_user_agent },
    { 0 } };

void init(Lumi *lumi){
    web_view = lumi->web_view;
    update_user_agent();
}
