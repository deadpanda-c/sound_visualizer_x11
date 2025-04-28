#include <X11/Xlib.h>
#include <unistd.h>

int main() {
    Display* dpy = XOpenDisplay(NULL);
    if (!dpy) return 1;

    int screen = DefaultScreen(dpy);
    Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
                                     100, 100, 800, 400, 1,
                                     BlackPixel(dpy, screen),
                                     WhitePixel(dpy, screen));

    XMapWindow(dpy, win);
    XFlush(dpy);

    // Run loop
    while (true) {
        XEvent ev;
        XNextEvent(dpy, &ev);
        if (ev.type == Expose) {
            // Draw stuff here later
        }
        usleep(16000); // ~60fps
    }

    XCloseDisplay(dpy);
    return 0;
}
