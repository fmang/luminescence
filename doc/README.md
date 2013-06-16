% [Luminescence]'s README
% Frédéric Mangano
% April 21, 2012

[Luminescence]: index.html

Installing
----------

For Arch Linux users, an [AUR][] package is available. I also have my [custom repository][repo].

[AUR]: https://aur.archlinux.org/packages.php?ID=57930
[repo]: http://mg0.fr/arch

To install from source:

    make
    make install

(the default `DESTDIR` is `/usr/local`, as usual)

Plugins
-------

Plugins are read in alphabetical order (it does matter) from `~/.luminescence/plugins`.

By default, no plugins are compiled.
Instead, their source files are stored in `/usr/src/luminescence/plugins`.

The `luminescence-install` script takes C plugin files and installs them.
For example, to install all the available plugins:

    luminescence-install /usr/src/luminescence/plugins/*.c

Commands
--------

You can list the available commands with:

    luminescence --help

To execute commands from the command line:

    luminescence --scripts off --user-agent Luminescence

Configuration (exec.c)
----------------------

The default configuration file is stored in `~/.luminescence/config`:

    # Sample configuration file
    user-agent "Mozilla/5.0 Luminescence"
    scripts off
    uri http://foo/homepage

Alternatively, you can configure Luminescence directly from C. Have a look at `luminescence.h`.

Key bindings (bind.c)
--------------------

You'll want to set up key bindings. The command is `bind`.
For example, you can write into `~/.luminescence/config`:

    bind i insert-mode
    bind u uri-edit
    bind r reload
    bind j scripts off
    bind J scripts on
    bind / find

Aliases (alias.c)
-----------------

You may want to hide the URI label when editing the URI.
All you have to do is run:

    on uri-edit uri-hide

The `alias` command is similar to `on` except that is relays the arguments.
You can use it to create synonyms or shortcuts.
