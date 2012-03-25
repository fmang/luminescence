#include "luminescence.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <getopt.h>

Lumi lumi;

void **plugins = 0;
int plugin_count = 0;

KeyCallback **key_callbacks = 0;
KeyCallback *key_grabber = 0;
int key_callback_count = 0;

Option *options = 0;
int option_count = 0;

bool on_key_press(GtkWidget *widget, GdkEventKey *event){
    int i = 0, code;
    for(; i<key_callback_count; i++){
        code = (*(key_grabber ? key_grabber : key_callbacks[i]))(event);
        if(code & FOCUS_GRAB){
            if(!key_grabber) key_grabber = key_callbacks[i];
            return code & EVENT_STOP ? TRUE : FALSE;
        }
        else if(key_grabber){
            key_grabber = 0;
            i--;
        }
        if(code & EVENT_STOP)
            return TRUE;
    }
    return TRUE;
}

void load_plugin(const char *path){
    void *plugin = dlopen(path, RTLD_LAZY);
    if(!plugin) return;

    plugins = (void**) realloc(plugins, sizeof(void*) * (plugin_count+1));
    plugins[plugin_count++] = plugin;

    // Key callback
    KeyCallback* plugin_key_callback = dlsym(plugin, "key_callback");
    if(plugin_key_callback){
        key_callbacks = (KeyCallback**) realloc(key_callbacks, sizeof(KeyCallback*) * (key_callback_count+1));
        key_callbacks[key_callback_count++] = plugin_key_callback;
    }

    // Options
    Option *plugin_options = dlsym(plugin, "options");
    if(plugin_options){
        size_t plugin_option_count = 0;
        while((plugin_options)[plugin_option_count].name)
            plugin_option_count++;
        options = (Option*) realloc(options, sizeof(Option) * (option_count + plugin_option_count));
        memcpy(options + option_count, plugin_options, sizeof(Option) * plugin_option_count);
        option_count += plugin_option_count;
    }
}

int is_visible(const struct dirent *entry){
    return entry->d_name[0] != '.';
}

void load_plugins(){
    struct dirent **entries;
    int entry_count = scandir("plugins", &entries, is_visible, alphasort);
    if(entry_count < 0) return;
    char filename[256];
    int i = 0;
    for(; i<entry_count; i++){
        strcpy(filename, "plugins/");
        strncat(filename, entries[i]->d_name, 247);
        load_plugin(filename);
        free(entries[i]);
    }
    free(entries);
}

void set_option(const char *opt, const char *val){
    int i = 0;
    for(; i<option_count; i++){
        if(strcmp(options[i].name, opt) == 0)
            break;
    }
    if(i == option_count)
        fprintf(stderr, "unknown option %s\n", opt);
    else if(options[i].argument == REQUIRED_ARGUMENT && !val)
        fprintf(stderr, "missing argument for option %s\n", opt);
    else{
        if(options[i].argument == NO_ARGUMENT && val)
            fprintf(stderr, "warning: option %s takes no argument\n", opt);
        options[i].callback(val);
    }
}

void load_config(){
    FILE *f = fopen("config", "r");
    if(!f) return;
    char *line = 0;
    size_t n = 0;
    char *name=0, *space;
    while(getline(&line, &n, f) != -1){
        if(line[0] == '\0') break; // empty
        if(line[0] == '\n' || line[0] == '#') continue; // comment
        if(line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0'; // drop the newline
        space = strchr(line, ' ');
        if(!space)
            set_option(line, 0);
        else{
            name = (char*) realloc(name, space-line+1);
            strncpy(name, line, space-line);
            set_option(name, space+1);
        }
    }
    free(line);
    if(name) free(name);
    fclose(f);
}

int main(int argc, char **argv){
    gtk_init(&argc, &argv);

    char *lumi_dir = strdup(getenv("HOME"));
    lumi_dir = realloc(lumi_dir, strlen(lumi_dir) + 15);
    strcat(lumi_dir, "/.luminescence");
    chdir(lumi_dir);

    // Window
    lumi.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_add_events(lumi.window, GDK_KEY_PRESS_MASK);
    gtk_window_set_has_resize_grip(GTK_WINDOW(lumi.window), FALSE);
    g_signal_connect(lumi.window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(lumi.window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    // Layout
    GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(lumi.window), layout);
    gtk_widget_show(layout);

    // Web view
    lumi.web_view = webkit_web_view_new();
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_window), lumi.web_view);
    gtk_box_pack_start(GTK_BOX(layout), scrolled_window, TRUE, TRUE, 0);
    gtk_widget_show_all(scrolled_window);

    // Status bar
    lumi.status_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(layout), lumi.status_bar, FALSE, FALSE, 0);
    gtk_widget_show(lumi.status_bar);

    // Plugins
    load_plugins();
    int i;
    void (*plugin_init)(Lumi*);
    for(i=0; i<plugin_count; i++){
        plugin_init = dlsym(plugins[i], "init");
        if(plugin_init) (*plugin_init)(&lumi);
    }

    // Options
    load_config();
    struct option *opts = (struct option*) malloc(sizeof(struct option*) * (option_count+1));
    opts[option_count].name = 0;
    int arg;
    for(i=0; i<option_count; i++){
        arg = options[i].argument;
        opts[i].name = options[i].name;
        opts[i].has_arg = arg == OPTIONAL_ARGUMENT ? optional_argument :
                          arg == REQUIRED_ARGUMENT ? required_argument : no_argument;
        opts[i].flag = 0;
    }
    while(i=-1, getopt_long(argc, argv, "", opts, &i) != -1){
        if(i != -1)
            options[i].callback(optarg);
    }
    free(opts);

    // Exec
    gtk_widget_show(lumi.window);
    gtk_main();

    // Clean up
    for(i=0; i<plugin_count; i++)
        dlclose(plugins[i]);

    return 0;
}
