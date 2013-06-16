% Key bindings for [Luminescence]
% Frédéric Mangano
% April 23, 2012

[Luminescence]: index.html

Introduction
------------

The `bind` plugin provides a vim-like interface for Luminescence (1.0.1 at the time of this writing). The source file for this plugin can be found [here][bind-git].

[bind-git]: http://git.mg0.fr/luminescence/tree/plugins/0-bind.c?id=2b40fbc2c8e3238b308dfad05733bee5b9af10f4

Quickstart
----------

To register key bindings, you should use the `bind` command.
You can either call it directly from the command-line, or write your bindings in the configuration file (see the [README](README.html) or the [`exec`] plugin).

    bind i insert-mode
    bind ^v uri-paste # ^ means Ctrl

The [`exec`]-style usage is:

    bind KEY COMMAND ARG1 ARG2 ...

[`exec`]: exec.html

Syntax
------

Technically, the `bind` command takes at least 2 arguments.

* The first is the key that should be bound.
* The second is the name of the command to call when the key is pressed.
* The other arguments are the arguments that will be passed to the command.

### Keys

The key is usually a single letter, and the case *is* important.

There is also the `^` modifer to match the Ctrl modifier.

Currently, other special keys (like Escape or Backspace) are not meant to be bound.
However you can probably bind them the right non-printable characters.
Otherwise you might as well add a few lines of code to bind the key you want.

Focus
-----

*Note: this section is for developers.*

When the `focus` command is called, `bind` relays key events to Gtk instead of handling them like it usually does. The user can release the focus by pressing the Escape key. When releasing the focus, `bind` calls the `leave` command, so that all the plugins which have registered the `leave` command will be aware of the focus release (which usually means "cancel").

In other words: you lose all control over the keyboard when focusing.
