#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

Display *display;						// Display
int screen;								// The screen (ID for the Window)
Window window;										// The window
GC gc;											// Graphics context
unsigned long black, white, red, green, blue;	// Colors



void init();
void close();
void draw();
unsigned long RGB(unsigned char r, unsigned char g, unsigned char b);

struct coord {
   int x, y;
} dot;

int main()
{
   init();
   XEvent event;
   KeySym key;
   char text[255];

   bool isRunning{true};
   while(isRunning)
   {
      XNextEvent(display, &event);
      if(event.type == Expose && event.xexpose.count == 0)
      {
         draw();
      }
      if(event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1)
      {
         if(text[0] == 'q')
         {
            close();
            isRunning = false;
         }
         std::printf("You pressed the %c key!\n", text[0]);
      }
      if(event.type == ButtonPress)
      {
         int x = event.xbutton.x, y = event.xbutton.y;
         XSetForeground(display, gc, red);
         XDrawLine(display, window, gc, dot.x, dot.y, x, y);
         XSetForeground(display, gc, blue);
         std::strcpy(text, "Hello World");
         XDrawString(display, window, gc, x, y, text, strlen(text));
      }
   }
   
   return 0;
}

void init()
{
   dot.x = 100; dot.y = 100;	// Starting point
   display = XOpenDisplay( static_cast<char*>(0) );	// Basically make XOpenDisplay be null
   screen = DefaultScreen(display);			// Make screen focused on display by default
   black = BlackPixel(display, screen);		// Colors
   white = WhitePixel(display, screen);		// Colors
   red = RGB(255, 0 , 0);			// Colors
   green = RGB(0, 255, 0);			// Colors
   blue = RGB(0, 0, 255);			// Colors
   window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 640, 480, 5, white, black);	// Create Window
   XSetStandardProperties(display, window, "X11-Server Window", "Hello", None, NULL, 0, NULL);	// Give Window properties
   XSelectInput(display, window, ExposureMask | ButtonPressMask | KeyPressMask);	// Handles detection of input
   gc = XCreateGC(display, window, 0, 0);	// Create that graphics context onto gc
   XSetBackground(display, gc, white);		// Not needed since already done in window, more knowledge though
   XSetForeground(display, gc, black);		// Not needed since already done in window, more knowledge though
   XClearWindow(display, window);			// Clears the window
   XMapRaised(display, window);				// Puts window ontop of things to do
}

void close()
{
   // Handles the window closing
   XFreeGC(display, gc);
   XDestroyWindow(display, window);
   XCloseDisplay(display);
   std::exit(0);
}

void draw()
{
   // Upon draw clear the window
   XClearWindow(display, window);
   
}

unsigned long RGB(unsigned char r, unsigned char g, unsigned char b)
{
   // Convert onto an unsigned long integer with bitwise shifting insertions
   // Handles colors
   return b + (g << 8) + (r << 16);
}
