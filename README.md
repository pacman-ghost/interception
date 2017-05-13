# MouseInterception

This tool generates mouse and keyboard events from mouse movement.

Why on earth would you want to do this? I've recently switched to using a trackball, and while they're nice, they almost always either have no scroll-wheel, or it's unusable. I had the idea of writing something that would capture mouse movement, and if a key was also pressed, translate that mouse movement into scrolling i.e. moving the mouse up and down while holding down Alt would scroll a document up and down. I do a lot of audio work, which often involves very large documents, horizontally as well as vertically, so being able to pan a document using Alt-MouseLeft/Right would also be very useful.

The code is in two parts, a C++ component (mouse.dll) that wraps the driver and passes information to and from a C# application (MouseInterception.exe) that handles the UI.

The project is still a WIP, but there are interim instructions for getting things up and running in <tt>$/_WIP_/</tt>.

### Mouse filter driver

Francisco Lopes has written a [filter driver](http://www.oblita.com/interception.html) that lets you capture low-level mouse and keyboard events, but IMHO, installing the keyboard driver is a serious security risk. He has said on several occasions that he doesn't agree, but this thing allows user-mode programs to bypass Windows security and capture all keyboard input e.g. password entry, even Windows logon passwords! And while the client library that talks with the driver is [open-source](https://github.com/oblitum/Interception), the driver itself is closed, so we can't modify it to add some access control.

However, after some discussion with Francisco, it seems it's possible to install only the mouse driver (see <tt>$/_ARCHIVE_/removal/</tt>). It's a bit messy, but it works, so we're good to go.

### Usability issues

The code currently works, although there's no UI yet, so you have to manage the config files yourself.

However, there are 2 usability issues:

* The code injects modified mouse messages into another application's message queue using <tt>SendInput()</tt>, but it turns out that a lot of programs don't respond well to this, and get confused as to whether a key is up or down. Alt is particularly problematic, since it's used to activate the main menu, so if a program thinks it's been pressed, the menu activates (browsers seem to be particularly susceptible to this). We could probably fix this by analyzing and modifying the raw keyboard input queue using the keyboard driver, but as explained above, this is not an option :-(

* I run everything in virtual machines, and VMware is doing its own funkiness with the mouse, and the two don't play well together. When run on the host machine, mouse messages get translated correctly, and a document scrolls without the mouse moving, but in a VM, it drifts slightly. Adding a fudge factor to try to nudge the mouse back sorta works, but not particularly well :-(
