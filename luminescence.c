#include "luminescence.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <getopt.h>

Lumi lumi;

typedef struct {
    void *handle;
    char *filename;
    const char *name;
    const char *description;
    void (*init)(Lumi*);
    KeyCallback *key_callback;
    Option *options;
} Plugin;

Plugin *plugins = 0;
int plugin_count = 0;

Option **options = 0;
int option_count = 0;

bool on_key_press(GtkWidget *widget, GdkEventKey *event){
    int i = 0, code;
    static KeyCallback *key_grabber = 0;
    for(; i<plugin_count; i++){
        if(!key_grabber && !plugins[i].key_callback) continue;
        code = (*(key_grabber ? key_grabber : plugins[i].key_callback))(event);
        if(code & FOCUS_GRAB){
            if(!key_grabber) key_grabber = plugins[i].key_callback;
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

void load_plugin(const char *filename){
    char path[256];
    strcpy(path, "plugins/");
    strncat(path, filename, 247);
    void *handle = dlopen(path, RTLD_LAZY);
    if(!handle) return;

    plugins = (Plugin*) realloc(plugins, sizeof(Plugin) * (plugin_count+1));
    Plugin *plugin = plugins + plugin_count++;
    plugin->handle = handle;
    plugin->filename = strdup(filename);
    const char **name = dlsym(handle, "name");
    plugin->name = name ? *name : 0;
    const char **description = dlsym(handle, "description");
    plugin->description = description ? *description : 0;
    plugin->init = dlsym(handle, "init");
    plugin->key_callback = dlsym(handle, "key_callback");

    // Options
    plugin->options = dlsym(handle, "options");
    if(plugin->options){
        Option *o = plugin->options;
        for(; o->name; o++){
            options = (Option**) realloc(options, sizeof(Option*) * (option_count+1));
            options[option_count++] = o;
        }
    }
}

int is_visible(const struct dirent *entry){
    return entry->d_name[0] != '.';
}

void load_plugins(){
    struct dirent **entries;
    int entry_count = scandir("plugins", &entries, is_visible, alphasort);
    if(entry_count < 0) return;
    int i = 0;
    for(; i<entry_count; i++){
        load_plugin(entries[i]->d_name);
        free(entries[i]);
    }
    free(entries);
}

void set_option(const char *opt, const char *val){
    int i = 0;
    for(; i<option_count; i++){
        if(strcmp(options[i]->name, opt) == 0)
            break;
    }
    if(i == option_count)
        fprintf(stderr, "unknown option %s\n", opt);
    else if(options[i]->argument == REQUIRED_ARGUMENT && !val)
        fprintf(stderr, "missing argument for option %s\n", opt);
    else{
        if(options[i]->argument == NO_ARGUMENT && val)
            fprintf(stderr, "warning: option %s takes no argument\n", opt);
        options[i]->callback(val);
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
    char *lumi_dir = strdup(getenv("HOME"));
    lumi_dir = realloc(lumi_dir, strlen(lumi_dir) + 15);
    strcat(lumi_dir, "/.luminescence");
    chdir(lumi_dir);

    load_plugins();

    // Help
    if(argc == 2){
        if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            puts("Usage: luminescence --OPTION[=VALUE] ...");
            if(!plugin_count){
                puts("No plugins.");
                return 0;
            }
            puts("Available plugins:");
            int i, j, pad;
            Option *o;
            int longest = 0;
            for(i=0; i<option_count; i++){
                if(strlen(options[i]->name) > longest)
                    longest = strlen(options[i]->name);
            }
            for(i=0; i<plugin_count; i++){
                fputs("* ", stdout);
                if(plugins[i].name)
                    printf("%s (%s)\n", plugins[i].name, plugins[i].filename);
                else
                    puts(plugins[i].filename);
                if(plugins[i].description)
                    puts(plugins[i].description);
                o = plugins[i].options;
                if(!o) continue;
                puts("Options:");
                for(; o->name; o++){
                    fputs("  ", stdout);
                    fputs(o->name, stdout);
                    if(o->description){
                        pad = longest - strlen(o->name) + 3;
                        for(j=0; j<pad; j++)
                            putchar(' ');
                        fputs(o->description, stdout);
                    }
                    putchar('\n');
                }
            }
            return 0;
        }
    }

    gtk_init(&argc, &argv);

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
    int i;
    for(i=0; i<plugin_count; i++)
        if(plugins[i].init) (*plugins[i].init)(&lumi);

    // Options
    load_config();
    struct option *opts = (struct option*) malloc(sizeof(struct option) * (option_count+1));
    opts[option_count].name = 0;
    int arg;
    for(i=0; i<option_count; i++){
        arg = options[i]->argument;
        opts[i].name = options[i]->name;
        opts[i].has_arg = arg == OPTIONAL_ARGUMENT ? optional_argument :
                          arg == REQUIRED_ARGUMENT ? required_argument : no_argument;
        opts[i].flag = 0;
    }
    while(i=-1, getopt_long(argc, argv, "", opts, &i) != -1){
        if(i != -1)
            options[i]->callback(optarg);
    }
    free(opts);

    // Exec
    gtk_widget_show(lumi.window);
    gtk_main();

    return 0;
}
