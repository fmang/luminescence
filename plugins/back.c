#include <luminescence.h>

const char *name = "Back";
const char *description = "Hit Backspace to go back one page.";

Lumi *lumi;

void go_back(){
    webkit_web_view_go_back(WEBKIT_WEB_VIEW(lumi->web_view));
}

Command commands[] = {
    { "back", go_back },
    { 0 } };

void init(){
    lumi_bind(GDK_KEY_BackSpace, "back", 0);
}
