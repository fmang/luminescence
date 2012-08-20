#include <luminescence.h>
#include <string.h>

const char *name = "Scroll";

Lumi *lumi;

#define ARG_CASE(str) if(strcasecmp(argv[1], (str)) == 0)

void scroll(int argc, char **argv){
    if(argc <= 1) return;
    GtkAdjustment *adjustment = argv[1][0] == 'l' || argv[1][0] == 'r'
        ? gtk_scrollable_get_hadjustment(GTK_SCROLLABLE(lumi->web_view))
        : gtk_scrollable_get_vadjustment(GTK_SCROLLABLE(lumi->web_view));
    gdouble value = gtk_adjustment_get_value(adjustment);
    gdouble step = gtk_adjustment_get_step_increment(adjustment);
    ARG_CASE("up")       value -= step;
    ARG_CASE("right")    value += step;
    ARG_CASE("down")     value += step;
    ARG_CASE("left")     value -= step;
    ARG_CASE("pageup")   value -= gtk_adjustment_get_page_increment(adjustment);
    ARG_CASE("pagedown") value += gtk_adjustment_get_page_increment(adjustment);
    ARG_CASE("top")      value = gtk_adjustment_get_lower(adjustment);
    ARG_CASE("bottom")   value = gtk_adjustment_get_upper(adjustment);
    gtk_adjustment_set_value(adjustment, value);
}

Command commands[] = {
    { "scroll", scroll, "up / right / down / left / pageup / pagedown / top / bottom" },
    { 0 } };
