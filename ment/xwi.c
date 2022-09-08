/*****************************************************************************/
/* Transport Interface to Xlib using MIT C-Bindings, Urs Rohrer, June 1991    /
/* Linux-Fortran interface for all callable routines.                         /
/* Caution: eventually compile with cc /noopt xwi                             /
/*****************************************************************************/

/*  Calling sequences in C:

void inixwi_(char *Name, float *x, float *y, float *w, float *h);
void erase_(void);
void eraser_(float *u1, float *v1, float *u2, float *v2);
void closer_(void);
void flush_xwi__(void);
void gramov_(float *x, float *y);
void gradrw_(float *x, float *y);
void set_col__(int *index);
void set_col_l__(int *index);
void lined_(int *width);
void linex_(int *l);
void text_out__(char *text, float *x, float *y, int *size);
void grasave_(void);
void getinp_(float *x, float *y, char *prompt, char *answer);
void getmpos_(float *x, float *y, int *nr);
void vt220_pop__(void);
void vt220_push__(void);
int  menu_(float *x, float *y, int *nx, int *ny, char *str, int len);
int  menu2_(float *x, float *y, int *nx, int *ny, char *str, int *marked,
            int len);
int  menu3_(float *x, float *y, int *nx, int *ny, char *str, char *tit,
            int *n, int len);
void marker_(float *x, float *y, float *w, float *h, int *cindex);
int  getinter_(void);

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

/*-------------------------------- Globals ----------------------------------*/
/*--------- static globals are only reachable inside this file --------------*/

/* #define TEST */ 

#define NP 500
#define NC   9
#define NM 100

static Display       *dpy;
static Window        window0, window1, panes[NM];
static Screen        *screen;
static GC            gc;
static GC            ngc, rgc;
static GC            gc_c_on, gc_c_off;
static XGCValues     xgcv;
static Font          fontm, font[4];
static XPoint        xy[NP];
static Cursor        fontcursor;
static Pixmap        pixmap;

static int           xycount = 0, image = 0, focus = 0, evnr;
static int           window1W, window1H, window1X, window1Y;
static int           xr, yr;
static unsigned int  wr, hr;
static char          input[100];
static int           input_len;
static int           xinp,yinp;
static unsigned int  color;
static unsigned long colind[NC];
static unsigned long menu_col_b, menu_col_f;
static int           foreground_index = 1;
static int           nrsave = -1;
static char          *labels[NM];

/*------------------------- static  procedures -------------------------------*/

static void doLoadFont(void)
{
   static char FontNameM[] = "*-COURIER-BOLD-R-NORMAL--*-100-*" ;
   static char FontName0[] = "*-HELVETICA-MEDIUM-R-NORMAL--*-100-*" ;
   static char FontName1[] = "*-HELVETICA-MEDIUM-R-NORMAL--*-120-*" ;
   static char FontName2[] = "*-HELVETICA-MEDIUM-R-NORMAL--*-140-*" ;
   static char FontName3[] = "*-HELVETICA-MEDIUM-R-NORMAL--*-240-*" ;

   fontm   = XLoadFont(dpy,FontNameM);
   font[0] = XLoadFont(dpy,FontName0);
   font[1] = XLoadFont(dpy,FontName1);
   font[2] = XLoadFont(dpy,FontName2);
   font[3] = XLoadFont(dpy,FontName3);
}

/*----------------------------------------------------------------------------*/

static void timer(void (*func)(void), int time)
{
   signal ( SIGALRM, (void (*)) func );
   alarm ( time );
}
/*----------------------------------------------------------------------------*/

static void wait(int timex)
{
   usleep(1000*timex);
}
/*----------------------------------------------------------------------------*/

static void doDefineColorTable(void)
{
/*
   0: black
   1: white
   2: red
   3: green
   4: blue
   5: cyan
   6: pink
   7: yellow
   8: dark-grey
*/
   int            i;
   Colormap       cmap;
   XColor         colors[NC];
   unsigned short rgb_r[] = {0,65535,65535,0    ,    0,0    ,48316,65535,771};
   unsigned short rgb_g[] = {0,65535,    0,65535,    0,65535,36751,65535,771};
   unsigned short rgb_b[] = {0,65535,    0,32639,65535,65535,36751,    0,771};

   if (color)
   {  cmap = XDefaultColormapOfScreen(screen);

      for (i=0; i<NC; i++)
      {  colors[i].flags = DoRed | DoGreen | DoBlue;
         colors[i].red   = rgb_r[i];
         colors[i].green = rgb_g[i];
         colors[i].blue  = rgb_b[i];
         if (XAllocColor(dpy,cmap,&colors[i]))
            colind[i] = colors[i].pixel;
         else
            printf("Color %d not allocated\n",i);
      }
   }
   else
   {   for (i=0; i<NC; i++)
          colind[i] = XWhitePixelOfScreen(screen);
       colind[0]    = XBlackPixelOfScreen(screen);
       colind[NC-1] = XBlackPixelOfScreen(screen);
   }
/*
   for menues
*/
   if (color)
   {  menu_col_f = colind[5];  /* cyan */
      menu_col_b = colind[2];  /* red  */
   }
   else
   {  menu_col_f = XWhitePixelOfScreen(screen);
      menu_col_b = XBlackPixelOfScreen(screen);
   }
}
/*----------------------------------------------------------------------------*/

static void doCreateGraphicsContext(void)
{
/*
   for text corsor
*/
   xgcv.line_style = LineSolid;
   xgcv.line_width = 1;
   xgcv.foreground = colind[2];     /* red */
   xgcv.background = colind[NC-1];  /* dark grey */
   gc_c_on = XCreateGC(dpy, window1,
                  GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                  &xgcv);

   xgcv.line_style = LineSolid;
   xgcv.line_width = 1;
   xgcv.foreground = colind[NC-1];  /* dark grey */
   xgcv.background = colind[NC-1];  /* dark grey */
   gc_c_off = XCreateGC(dpy, window1,
                  GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                  &xgcv);
/*
   drawing and text
*/
   xgcv.line_style = LineSolid;
   xgcv.line_width = 1;
   xgcv.foreground = colind[1];     /* white */
   xgcv.background = colind[NC-1];  /* dark grey */
   gc = XCreateGC(dpy, window1,
                  GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                  &xgcv);
/*
   for menus
*/
   ngc = XCreateGC(dpy,XRootWindowOfScreen(screen),0,NULL);
   XSetFont(dpy,ngc,fontm);
   XSetForeground(dpy,ngc,menu_col_b);
   rgc = XCreateGC(dpy,XRootWindowOfScreen(screen),0,NULL);
   XSetFont(dpy,rgc,fontm);
   XSetForeground(dpy,rgc,menu_col_f);
}
/*----------------------------------------------------------------------------*/

static void doCreateWindow(void)
{
   XSetWindowAttributes xswa;

   xswa.event_mask       = ExposureMask | ButtonPressMask | KeyPressMask |
                           PointerMotionMask | StructureNotifyMask;
   xswa.background_pixel = colind[NC-1];  /* dark grey */
   window1 = XCreateWindow(dpy,
                           XRootWindowOfScreen(screen),
                           window1X,window1Y,window1W,window1H,
                           0,
                           XDefaultDepthOfScreen(screen),
                           InputOutput,
                           XDefaultVisualOfScreen(screen),
                           CWEventMask | CWBackPixel,
                           &xswa);
}
/*----------------------------------------------------------------------------*/

static void doCreatePixmap(void)
/*
   create pixmap (drawable) to store content of window1 (backing-store)
*/
{
   pixmap = XCreatePixmap(dpy,XRootWindowOfScreen(screen),window1W,window1H,
                          DefaultDepthOfScreen(screen));
   XFillRectangle(dpy,pixmap,gc_c_off,0,0,window1W,window1H);
}
/*----------------------------------------------------------------------------*/

static void doWMHints(char *Window1Name)
{
   XSizeHints xsh;

   xsh.x      = window1X;
   xsh.y      = window1Y;
   xsh.width  = window1W;
   xsh.height = window1H;
   xsh.flags  = PPosition | PSize;

   XSetNormalHints(dpy,window1,&xsh);
   XStoreName(dpy,window1,Window1Name);
}
/*----------------------------------------------------------------------------*/

static void doMapWindow(void)
{
   XEvent event;

   XMapWindow(dpy,window1);
   for(;;)
   {  XNextEvent(dpy,&event);
      if (event.type == MapNotify)
         break;
   }
}
/*----------------------------------------------------------------------------*/

static void doExposeA(XEvent *eventP)
{
   int x, y, w, h;

   if (eventP->xexpose.window == window1)
      if (image)
      {  w = ((window1X + window1W) > (xr + wr)) ? (wr - window1X) : window1W;
         h = ((window1Y + window1H) > (yr + hr)) ? (hr - window1Y) : window1H;
         w = (window1X < 0) ? window1W + window1X : w;
         h = (window1Y < 0) ? window1H + window1Y : h;
         x = (window1X < 0) ? -window1X : 0;
         y = (window1Y < 0) ? -window1Y : 0;
         XCopyArea(dpy,pixmap,window1,gc,x,y,w,h,x,y);
      }
}
/*----------------------------------------------------------------------------*/

static void DrawCursor(void)
{
   int            dir, asc, des;
   XCharStruct    over;

   XQueryTextExtents(dpy,font[3],input,strlen(input),&dir,&asc,&des,&over);
   XFillRectangle(dpy,window1,gc_c_on,xinp+(int)over.width,yinp-28,20,30);
}
/*----------------------------------------------------------------------------*/

static void enable(void)
{
   image = 1;
}
/*----------------------------------------------------------------------------*/

static void EraseCursor(void)
{
   int            dir, asc, des;
   XCharStruct    over;

   XQueryTextExtents(dpy,font[3],input,strlen(input),&dir,&asc,&des,&over);
   XFillRectangle(dpy,window1,gc_c_off,xinp+(int)over.width,yinp-28,20,30);
}
/*----------------------------------------------------------------------------*/

static void DrawMarker(Window win, int x, int y)
{
   XPoint xy[3];

   xy[0].x = x;
   xy[0].y = y;
   xy[1].x = x;
   xy[1].y = y + 10;
   xy[2].x = x - 10;
   xy[2].y = y + 5;
   XFillPolygon(dpy,win,ngc,xy,3,Convex,CoordModeOrigin);
}
/*----------------------------------------------------------------------------*/

static void EraseMarker(Window win, int x, int y)
{
   XPoint xy[3];

   xy[0].x = x;
   xy[0].y = y;
   xy[1].x = x;
   xy[1].y = y + 10;
   xy[2].x = x - 10;
   xy[2].y = y + 5;
   XFillPolygon(dpy,win,rgc,xy,3,Convex,CoordModeOrigin);
}
/*----------------------------------------------------------------------------*/

static int doButtonPressA(XEvent *eventP)
{
   char         buffer[15];
   Window       root,  child;
   int          rootx, rooty;
   int          winx,  winy;
   unsigned int keyb;
   int          retkey = 0;

   if (eventP->xexpose.window == window1)
   {  XQueryPointer(dpy,window1,&root,&child,&rootx,&rooty,&winx,&winy,&keyb);
      sprintf(buffer,"%7.1f%7.1f",(float)winx,(float)(window1H-winy));
      buffer[14] = '\0';
      input[input_len] = '\0';
      if (eventP->xbutton.button == Button1)
      {  EraseCursor();
         strcat(input,"MOUSE1");
         strcat(input,buffer);
      }
      if (eventP->xbutton.button == Button2)
      {  EraseCursor();
         strcat(input,"MOUSE2");
         strcat(input,buffer);
      }
      if (eventP->xbutton.button == Button3)
      {  EraseCursor();
         strcat(input,"MOUSE3");
         strcat(input,buffer);
      }
      retkey = 1;
   }
   return retkey;
}
/*----------------------------------------------------------------------------*/

static int doKeyPressA(XEvent *eventP)
{
   KeySym         sym;
   XComposeStatus comp;
   char           buffer[10], *string;
   int            flag, key;
   static int     controlpressed = 0;

   if (eventP->xkey.window == window1)
   {  XLookupString((XKeyEvent *)eventP,buffer,10,&sym,&comp);
      string = XKeysymToString(sym);
      key = XKeycodeToKeysym(dpy,eventP->xkey.keycode,0);

      if ( !strcmp(string,"Return") )
      {  EraseCursor();
         flag = 1;
      }
      else if ( !strcmp(string,"Delete") && (strlen(input) > input_len) )
      {  EraseCursor();
         input[strlen(input)-1] = '\0';
         strcat(input,"   ");
         XDrawImageString(dpy,window1,gc,xinp,yinp,input,strlen(input));
         input[strlen(input)-3] = '\0';
         DrawCursor();
         flag = 0;
      }
      else if ( !strcmp(string,"BackSpace") && (strlen(input) > input_len) )
      {  EraseCursor();
         input[strlen(input)-1] = '\0';
         strcat(input,"   ");
         XDrawImageString(dpy,window1,gc,xinp,yinp,input,strlen(input));
         input[strlen(input)-3] = '\0';
         DrawCursor();
         flag = 0;
      }
      else if (controlpressed)
      {  controlpressed = 0;
         flag = 0;
         if ( !strcmp(string,"z") || !strcmp(string,"Z") )
         {  EraseCursor();
            input[input_len] = '\0';
            strcat(input,"CONTROL_Z");
            flag = 1;
         }
      }
      else if ( !strcmp(string,"Control_L") )
      {  controlpressed = 1;
         flag = 0;
      }
      else if (key > 31 && key < 127)
      {  buffer[1] = '\0';
         EraseCursor();
         strcat(input,buffer);
         XDrawImageString(dpy,window1,gc,xinp,yinp,input,strlen(input));
         DrawCursor();
         flag = 0;
      }
      else
         flag = 0;
   }
   return flag;
}
/*----------------------------------------------------------------------------*/

static void GetMovement(XEvent *eventP)
/*
   Get new x and y of translated window (D. Anicic, CopyImage3.C)
*/
{
   if (eventP->xclient.message_type == 78)    /* event 78 is undocumented */
   {  window1X = eventP->xclient.data.s[0];
      window1Y = eventP->xclient.data.s[1];
   }
}
/*----------------------------------------------------------------------------*/

static void GetConfigure(XEvent *eventP)
{
/*
   Get new y. w and h of resized window (D. Anicic, CopyImage3.C)
*/
   window1X = window1X + window1W - eventP->xconfigure.width;
   window1Y = window1Y + window1H - eventP->xconfigure.height;
   window1W = eventP->xconfigure.width;
   window1H = eventP->xconfigure.height;
}
/*----------------------------------------------------------------------------*/

static void doHandleEventsA(void)
{
   XEvent event;
   int    retkey = 0;

   for(;;)
   {  XNextEvent(dpy,&event);
/*      doTraceEvent(&event); */
      switch (event.type)
      {   case Expose:
            doExposeA(&event);
            break;
          case ButtonPress:
            retkey = doButtonPressA(&event);
            break;
          case ButtonRelease:
            break;
          case KeyPress:
            retkey = doKeyPressA(&event);
            break;
          case KeyRelease:
            break;
          case MotionNotify:
            break;
          case ClientMessage:
/*            GetMovement(&event); */
            break;
          case ConfigureNotify:
/*            GetConfigure(&event); */
            break;
          default:
            break;
      }
      if (retkey)
         break;
   }
}
/*----------------------------------------------------------------------------*/

static int paint_pane(Window window, int y, int mode)
{
   int win;
   int x = 2;
   GC  gc;

   if (mode == 1)
   {  XSetWindowBackground(dpy,window,menu_col_b);
      gc = rgc;
   }
   else
   {  XSetWindowBackground(dpy,window,menu_col_f);
      gc = ngc;
   }
   XClearWindow(dpy,window);
   for (win=0; window!=panes[win]; win++)
       ;
   XDrawString(dpy,window,gc,x,y,labels[win],strlen(labels[win]));
   return win;
}
/*----------------------------------------------------------------------------*/

static Bool predproc(Display *dpy, XEvent *event, char *arg)
/*
   return 1, if a mouse button or a key is pressed, otherwise return 0.
*/
{
   switch(event->type)
   {  case ButtonPress:
        evnr = event->xbutton.button;
        return(True);
        break;
      case KeyPress:
        evnr = XKeycodeToKeysym(dpy,event->xkey.keycode,0);
        return(True);
        break;
      default:
        return(False);
        break;
   }
}

/*--------------------- procedures callable from outside ---------------------*/
/*----------------- all parameters passed by reference (FORTRAN) -------------*/

void inixwi_(char *Name, float *x, float *y, float *w, float *h)
{
   char         *Window1Name;
   int          revert;
   Window       root;
   unsigned int b, d;

   Window1Name = Name;
   dpy = XOpenDisplay(0);
   if (!dpy)
   {  printf("Display not opened!\n");
      exit(-1);
   }
   window1X = (int) *x;
   window1Y = (int) *y;
   window1W = (int) *w;
   window1H = (int) *h;
   XGetInputFocus(dpy,&window0,&revert);    /* window0 = DecTerm window */
   screen   = XDefaultScreenOfDisplay(dpy);
   XGetGeometry(dpy,XRootWindowOfScreen(screen),&root,&xr,&yr,&wr,&hr,&b,&d);

   color = 1;
   doDefineColorTable();
   doLoadFont();

   doCreateWindow();
   doCreateGraphicsContext();
   fontcursor = XCreateFontCursor(dpy,XC_cross);
   XDefineCursor(dpy,window1,fontcursor);
   doWMHints(Window1Name);
   doMapWindow();
   doCreatePixmap();
}
/*----------------------------------------------------------------------------*/

void erase_(void)
{
   XClearWindow(dpy,window1);
}
/*----------------------------------------------------------------------------*/

void eraser_(float *u1, float *v1, float *u2, float *v2)
{
   int          x, y;
   unsigned int w, h;

   x = (int) *u1;
   y = window1H - (int) *v2;
   w = (int) (*u2 - *u1);
   h = (int) (*v2 - *v1);
   XClearArea(dpy,window1,x,y,w,h,0);
}
/*----------------------------------------------------------------------------*/

void closer_(void)
{
   XEvent event;

   XUnmapWindow(dpy,window1);
   for(;;)
   {  XNextEvent(dpy,&event);
      if (event.type == UnmapNotify)
         break;
   }
   XDestroyWindow(dpy,window1);
   XCloseDisplay(dpy);
}
/*----------------------------------------------------------------------------*/

void flush_xwi_(void)
{
   if (xycount)
   {  XDrawLines(dpy,window1,gc,xy,xycount,CoordModeOrigin);
      xycount = 0;
      XFlush(dpy);
   }
}
/*----------------------------------------------------------------------------*/

void gramov_(float *x, float *y)
{
   flush_xwi_();
   xy[xycount].x   = (int) *x;
   xy[xycount++].y = window1H - (int) *y;
}
/*----------------------------------------------------------------------------*/

void gradrw_(float *x, float *y)
{
   xy[xycount].x   = (int) *x;
   xy[xycount++].y = window1H - (int) *y;
   if (xycount == NP-1)
   {  flush_xwi_();
      xy[xycount].x   = (int) *x;
      xy[xycount++].y = window1H - (int) *y;
   }
}
/*----------------------------------------------------------------------------*/

void set_col_(int *index)
{
   flush_xwi_();
   if (*index < 0 || *index > NC - 1)
      printf("Color index %d out of range\n",*index);
   else
   {  xgcv.foreground = colind[*index];
      XChangeGC(dpy,gc,GCForeground,&xgcv);
      foreground_index = *index;
   }
}
/*----------------------------------------------------------------------------*/

void set_col_l_(int *index)
{
   flush_xwi_();
   if (*index < 0 || *index > NC - 1)
      printf("Color index %d out of range\n",*index);
   else
   {  xgcv.foreground = colind[*index];
      XChangeGC(dpy,gc,GCForeground,&xgcv);
      foreground_index = *index;
   }
}
/*----------------------------------------------------------------------------*/

void lined_(int *width)
{
   if (*width < 0)
      printf("Line thickness %d out of range\n",*width);
   else
   {  xgcv.line_width = *width;
      XChangeGC(dpy,gc,GCLineWidth,&xgcv);
   }
}
/*----------------------------------------------------------------------------*/

void linex_(int *l)
{
   char dash_list[] = {2,2};

   if (*l == 1)
   {  xgcv.line_style = LineOnOffDash;
      XChangeGC(dpy,gc,GCLineStyle,&xgcv);
      XSetDashes(dpy,gc,0,dash_list,2);
   }
   else
   {  xgcv.line_style = LineSolid;
      XChangeGC(dpy,gc,GCLineStyle,&xgcv);
   }
}
/*----------------------------------------------------------------------------*/

void text_out_(char *text, float *x, float *y, int *nr)
{
   char           *txt;
   unsigned short len, i;

   if (nrsave != *nr)
   {  XSetFont(dpy,gc,font[*nr]);
      nrsave = *nr;
   }
   txt = text;
   len = strlen(txt);
/*
   if (len > 1)
   {  for (i=len-1; i>=0; i--)
         if (txt[i] != ' ')
            break;
      len = i+1;
   }
*/
   XDrawImageString(dpy,window1,gc,(int)*x,window1H-(int)*y,txt,len);
}
/*----------------------------------------------------------------------------*/

void grasave_(void)
{
   int x, y, w, h;

   w = ((window1X + window1W) > (xr + wr)) ? (wr - window1X) : window1W;
   h = ((window1Y + window1H) > (yr + hr)) ? (hr - window1Y) : window1H;
   w = (window1X < 0) ? window1W + window1X : w;
   h = (window1Y < 0) ? window1H + window1Y : h;
   x = (window1X < 0) ? -window1X : 0;
   y = (window1Y < 0) ? -window1Y : 0;

   XCopyArea(dpy,window1,pixmap,gc,x,y,w,h,x,y);
   image = 1;
}
/*----------------------------------------------------------------------------*/

void getinp_(float *x, float *y, char *prompt, char *string)
{
   XCharStruct over;
   int         i, dir, asc, des, leng, height;
   char        *prmt, *str;

   prmt      = prompt;
   input_len = strlen(prompt);

   XQueryTextExtents(dpy,font[3],prmt,input_len,&dir,&asc,&des,&over);
   height = over.ascent + over.descent;

   xinp = (int) *x;
   yinp = window1H - (int) *y + height;

   strncpy(input,prmt,input_len);
   input[input_len] = '\0';
   XSetFont(dpy,gc,font[3]);         /* big font */
   XDrawImageString(dpy,window1,gc,xinp,yinp,input,input_len);
   grasave_();
   DrawCursor();
   image = 0;                        /* disable redraw */
   timer(enable,1);                  /* enable redraw after 1 sec */

   if (!focus)
   {  XSetInputFocus(dpy,window1,RevertToParent,CurrentTime);
      focus = 1;
   }
   doHandleEventsA();
   if (nrsave != -1)
      XSetFont(dpy,gc,font[nrsave]); /* old font */

   /* str = string; */
   /* for (i=0; i<strlen(string); str[i++]=' ') */  /* fill with blanks, Fortran */
   str = string;
   strncpy(str,&input[input_len],strlen(&input[input_len]));
}
/*----------------------------------------------------------------------------*/

void GetMpos_(float *x, float *y, int *nr)
{
   XEvent       event;
   Window       root,  child;
   int          rootx, rooty;
   int          winx,  winy;
   unsigned int keyb;

   while (1)
   {  XNextEvent(dpy,&event);
      switch (event.type)
      {   case Expose:
            break;
          case ButtonPress:
            if (event.xexpose.window == window1)
            {  XQueryPointer(dpy,window1,&root,&child,&rootx,&rooty,
                             &winx,&winy,&keyb);
               *x = (float)winx;
               *y = (float)(window1H-winy);
               if (event.xbutton.button == Button1)
                  *nr = 1;
               if (event.xbutton.button == Button2)
                  *nr = 2;
               if (event.xbutton.button == Button3)
                  *nr = 3;
               return;
            }
            break;
          case MotionNotify:
            break;
          default:
            break;
      }
   }
}
/*----------------------------------------------------------------------------*/

void vt220_pop_(void)
{
   XSynchronize(dpy,1);
   grasave_();
   XLowerWindow(dpy,window1);
   if (focus)
   {  XSetInputFocus(dpy,window0,RevertToParent,CurrentTime);
      focus = 0;
   }
   XSynchronize(dpy,0);
}
/*----------------------------------------------------------------------------*/

vt220_push_(void)
{
   XSynchronize(dpy,1);
   if (!focus)
   {  XSetInputFocus(dpy,window1,RevertToParent,CurrentTime);
      focus = 1;
   }
   XRaiseWindow(dpy,window1);
   if (image)
   {  wait(100);   /* 0.1 sec delay */
      XCopyArea(dpy,pixmap,window1,gc,0,0,window1W,window1H,0,0);
   }
   XSynchronize(dpy,0);
}
/*----------------------------------------------------------------------------*/

int menu_(float *x, float *y, int *nx, int *ny, char *str, int len)
/*

x       : x-coordinate of menu window (middle of menu)
y       : y-coordinate of menu window (top)
nx      : width of menu window in chars
ny      : number od lines in window
str     : array of ny text-strings (length <= nx)
returns : number of line clicked at
len     : auxiliary argument added by FORTRAN
*/
{
   int            i, l, n, nold = -1, yw, winindex, but, cont = 1;
   int            xpos, ypos, menu_width, menu_height, pane_height;
   char           *ptr;
   int            dir, asc, des;
   unsigned int   button;
   XCharStruct    over;
   Window         menuwin, win;
   XEvent         event;

   l    = len;
   ptr  = str;

   for (i=0; i<*ny; i++)
   {  labels[i] = (char *) malloc(l+1);
      strncpy(labels[i],ptr,l);
      labels[i][l] = '\0';
      ptr += l;
   }

   XQueryTextExtents(dpy,fontm,labels[0],strlen(labels[0]),
                     &dir,&asc,&des,&over);
   menu_width  = over.width + 4;
   pane_height = over.ascent + over.descent + 8;
   menu_height = pane_height * (*ny);

   xpos = (int)*x - menu_width/2;
   xpos = (xpos < 0) ? 0 : xpos;
   xpos = (xpos > window1W - menu_width) ? window1W - menu_width : xpos;

   ypos = window1H - (int)*y;
   ypos = (ypos < 0) ? 0 : ypos;
   ypos = (ypos > window1H - menu_height) ? window1H - menu_height : ypos;

   menuwin = XCreateSimpleWindow(dpy,
                                 window1,
                                 xpos, ypos,
                                 menu_width,menu_height,
                                 4,
                                 colind[1],
                                 menu_col_f);

   for (winindex=0; winindex<*ny; winindex++)
   {   yw = menu_height * winindex / (*ny);
       panes[winindex] = XCreateSimpleWindow(dpy,
                                             menuwin,
                                             0, yw, menu_width,pane_height,
                                             0,
                                             colind[1],
                                             menu_col_f);
       XSelectInput(dpy,panes[winindex],
       ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
   }

   XSelectInput(dpy,XRootWindowOfScreen(screen),SubstructureNotifyMask);
   XMapSubwindows(dpy,menuwin);

   XMapWindow(dpy,menuwin);

   while (cont)
   {  XNextEvent(dpy,&event);
      switch (event.type)
      {   case Expose:
            win = event.xexpose.window;
            if (win == window1)
            {  if (image)
                  XCopyArea(dpy,pixmap,window1,gc,0,0,window1W,window1H,0,0);
            }
            else
            {  for (i=0; i<(*ny); i++)
                  if ( win == panes[i] )
                  {  paint_pane(win,over.ascent+2,0);
                     break;
                  }
            }
            break;
          case ButtonPress:
            button = event.xbutton.button;
            win    = event.xbutton.window;
            for (i=0; i<(*ny); i++)
               if ( win == panes[i] )
               {  n = i;
                  cont = 0;
                  break;
               }
            break;
          case MotionNotify:
            n = -1;
            for (i=0; i<(*ny); i++)
               if (event.xmotion.window == panes[i])
               {  n = i;
                  break;
               }
            if (n != nold)
            {  if (nold != -1)
                   paint_pane(panes[nold],over.ascent+2,0);
               if (n != -1)
                  paint_pane(panes[n],over.ascent+2,1);
            }
            nold = n;
            break;
          default:
            break;
      }
   }

   if (!focus)
   {  XSetInputFocus(dpy,window1,RevertToParent,CurrentTime);
      focus = 1;
   }
   XUnmapWindow(dpy,menuwin);
   XDestroyWindow(dpy,menuwin);

   for (i=0; i<*ny; i++)
      free(labels[i]);

   if (image)
      XCopyArea(dpy,pixmap,window1,gc,0,0,window1W,window1H,0,0);

   return n+1;
}
/*----------------------------------------------------------------------------*/

int menu2_(float *x,float *y, int *nx, int *ny, char *str, int nr[], int len)
/*

x       : x-coordinate of menu window (middle of menu)
y       : y-coordinate of menu window (top)
nx      : width of menu window in chars
ny      : number od lines in window
str     : array of ny text-strings (length <= nx)
nr      : array containing numbers of lines marked with an arrow
returns : number of marked lines
len     : auxiliary arument added by FORTRAN

*/
{
   int            i, j, l, n, nold = -1, yw, winindex, but, cont = 1;
   int            xpos, ypos, menu_width, menu_height, pane_height, off = 20;
   char           *ptr;
   int            dir, asc, des, checked[100], nn=0;
   unsigned int   button;
   XCharStruct    over;
   Window         menuwin, win;
   XEvent         event;
/*
   get labels
*/
   l    = len;
   ptr  = str;

   for (i=0; i<*ny; i++)
   {  labels[i] = (char *) malloc(l+1);
      strncpy(labels[i],ptr,l);
      labels[i][l] = '\0';
      ptr += l;
   }
/*
   get checked labels
*/
   for (i=0; i<*ny-1; checked[i++]=0)
      ;
   for (i=0; i<*ny-1; i++)
      if ((j = nr[i]) != 0)
         checked[j-1] = 1;

   XQueryTextExtents(dpy,fontm,labels[0],strlen(labels[0]),
                     &dir,&asc,&des,&over);
   menu_width  = over.width + 4;
   pane_height = over.ascent + over.descent + 8;
   menu_height = pane_height * (*ny);

   xpos = (int)*x - menu_width/2;
   xpos = (xpos < 0) ? 0 : xpos;
   xpos = (xpos > window1W - menu_width - off) 
          ? window1W - menu_width - off : xpos;

   ypos = window1H - (int)*y;
   ypos = (ypos < 0) ? 0 : ypos;
   ypos = (ypos > window1H - menu_height) ? window1H - menu_height : ypos;

   menuwin = XCreateSimpleWindow(dpy,
                                 window1,
                                 xpos, ypos,
                                 menu_width+off,menu_height,
                                 4,
                                 colind[1],
                                 menu_col_f);

   for (winindex=0; winindex<*ny; winindex++)
   {   yw = menu_height * winindex / (*ny);
       panes[winindex] = XCreateSimpleWindow(dpy,
                                             menuwin,
                                             0, yw, menu_width,pane_height,
                                             0,
                                             colind[1],
                                             menu_col_f);
       XSelectInput(dpy,panes[winindex],
       ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
   }

   XSelectInput(dpy,XRootWindowOfScreen(screen),SubstructureNotifyMask);
   XMapSubwindows(dpy,menuwin);

   XMapWindow(dpy,menuwin);

   while (cont)
   {  XNextEvent(dpy,&event);
      switch (event.type)
      {   case Expose:
            win = event.xexpose.window;
            if (win == window1)
            {  if (image)
                  XCopyArea(dpy,pixmap,window1,gc,0,0,window1W,window1H,0,0);
            }
            else
            {  for (i=0; i<(*ny); i++)
                  if ( win == panes[i] )
                  {  paint_pane(win,over.ascent+2,0);
                     if ( i<(*ny)-1 && checked[i] )
                     {  DrawMarker(menuwin,menu_width+15,pane_height*i+2);
                        nn++;
                     }
                     break;
                  }
            }
            break;
          case ButtonPress:
            button = event.xbutton.button;
            win    = event.xbutton.window;
            for (i=0; i<(*ny); i++)
               if ( win == panes[i] )
                  if (i == (*ny)-1)
                  {  cont = 0;
                     break;
                  }
                  else
                  {  if (checked[i] == 1)
                     {  checked[i] = 0;
                        nn--;
                        EraseMarker(menuwin,menu_width+15,pane_height*i+2);
                     }
                     else
                     {  checked[i] = 1;
                        nn++;
                        DrawMarker(menuwin,menu_width+15,pane_height*i+2);
                     }
                  }   
         break;
          case MotionNotify:
            n = -1;
            for (i=0; i<(*ny); i++)
               if (event.xmotion.window == panes[i])
               {  n = i;
                  break;
               }
            if (n != nold)
            {  if (nold != -1)
                   paint_pane(panes[nold],over.ascent+2,0);
               if (n != -1)
                  paint_pane(panes[n],over.ascent+2,1);
            }
            nold = n;
            break;
          default:
            break;
      }
   }

   if (!focus)
   {  XSetInputFocus(dpy,window1,RevertToParent,CurrentTime);
      focus = 1;
   }
   XUnmapWindow(dpy,menuwin);
   XDestroyWindow(dpy,menuwin);
/*
   free labels
*/
   for (i=0; i<*ny; i++)
      free(labels[i]);
/*
   refresh screen
*/
   if (image)
      XCopyArea(dpy,pixmap,window1,gc,0,0,window1W,window1H,0,0);
/*
   return numbers of checked labels
*/
   for (i=0; i<*ny-1; nr[i++]=0)
      ;
   for (i=0,j=0; i<*ny-1; i++)
      if (checked[i])
         nr[j++] = i + 1;

   return nn;
}
/*----------------------------------------------------------------------------*/

int menu3_(float *x,float *y, int *nx, int *ny, char *str, char *titel,
           int *nr, int len)
/*

x       : x-coordinate of menu window (middle of menu)
y       : y-coordinate of menu window (top)
nx      : width of menu window in chars
ny      : number od lines in window
str     : array of ny text-strings (length <= nx)
titel   : menu title
nr      : line number marked with arrow
returns : number of line clicked at
len     : auxiliary argument added by FORTRAN

*/
{
   int            xpos, ypos, i, l, n, nold = -1, yw, winindex, but, cont = 1;
   int            menu_width, menu_height, pane_height, pos, off = 20;
   char           *ptr, tit[40];
   int            dir, asc, des;
   unsigned int   button;
   XCharStruct    over, over2;
   Window         menuwin, win, wintit;
   XEvent         event;


   l    = len;
   ptr  = str;

   for (i=0; i<*ny; i++)
   {  labels[i] = (char *) malloc(l+1);
      strncpy(labels[i],ptr,l);
      labels[i][l] = '\0';
      ptr += l;
   }

   XQueryTextExtents(dpy,fontm,labels[0],strlen(labels[0]),
                     &dir,&asc,&des,&over);
   menu_width  = over.width + 4;
   pane_height = over.ascent + over.descent + 8;
   menu_height = pane_height * (*ny + 1);

   strcpy(tit,titel);
   XQueryTextExtents(dpy,fontm,tit,strlen(tit),&dir,&asc,&des,&over2);
   pos = (menu_width + off - over2.width) / 2;   /* centering titel */
   if (pos < 2)
      pos = 2;

   xpos = (int)*x - menu_width/2;
   xpos = (xpos < 0) ? 0 : xpos;
   xpos = (xpos > window1W - menu_width - off) 
          ? window1W - menu_width - off : xpos;

   ypos = window1H - (int)*y;
   ypos = (ypos < 0) ? 0 : ypos;
   ypos = (ypos > window1H - menu_height) ? window1H - menu_height : ypos;

   menuwin = XCreateSimpleWindow(dpy,
                                 window1,
                                 xpos, ypos,
                                 menu_width+off,menu_height,
                                 4,
                                 colind[1],
                                 menu_col_f);

   for (winindex=0; winindex<*ny; winindex++)
   {   yw = pane_height + (menu_height-pane_height) * winindex / (*ny);
       panes[winindex] = XCreateSimpleWindow(dpy,
                                             menuwin,
                                             0, yw, menu_width,pane_height,
                                             0,
                                             colind[1],
                                             menu_col_f);
       XSelectInput(dpy,panes[winindex],
       ExposureMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
   }
   wintit = XCreateSimpleWindow(dpy,
                                menuwin,
                                0, 0, menu_width+off, pane_height,
                                0,
                                colind[1],
                                menu_col_f);
   XSelectInput(dpy,wintit,ExposureMask);

   XSelectInput(dpy,XRootWindowOfScreen(screen),SubstructureNotifyMask);
   XMapSubwindows(dpy,menuwin);

   XMapWindow(dpy,menuwin);

   while (cont)
   {  XNextEvent(dpy,&event);
      switch (event.type)
      {   case Expose:
            win = event.xexpose.window;
            if (win == window1)
            {  if (image)
                  XCopyArea(dpy,pixmap,window1,gc,0,0,window1W,window1H,0,0);
            }
            else if (win == wintit)
            {  XSetWindowBackground(dpy,wintit,menu_col_b);
               XClearWindow(dpy,wintit);
               XDrawString(dpy,wintit,rgc,pos,over.ascent+2,tit,strlen(tit));
            }
            else
            {  for (i=0; i<(*ny); i++)
                  if ( win == panes[i] )
                  {  paint_pane(win,over.ascent+2,0);
                     if (i == (*nr)-1)
                        DrawMarker(menuwin,menu_width+15,pane_height*(i+1)+2);
                     break;
                  }
            }
            break;
          case ButtonPress:
            button = event.xbutton.button;
            win    = event.xbutton.window;
            for (i=0; i<(*ny); i++)
               if ( win == panes[i] )
               {  n = i;
                  cont = 0;
                  break;
               }
            break;
          case MotionNotify:
            n = -1;
            for (i=0; i<(*ny); i++)
               if (event.xmotion.window == panes[i])
               {  n = i;
                  break;
               }
            if (n != nold)
            {  if (nold != -1)
                   paint_pane(panes[nold],over.ascent+2,0);
               if (n != -1)
                  paint_pane(panes[n],over.ascent+2,1);
            }
            nold = n;
            break;
          default:
            break;
      }
   }

   if (!focus)
   {  XSetInputFocus(dpy,window1,RevertToParent,CurrentTime);
      focus = 1;
   }
   XUnmapWindow(dpy,menuwin);
   XDestroyWindow(dpy,menuwin);

   for (i=0; i<*ny; i++)
      free(labels[i]);

   if (image)
      XCopyArea(dpy,pixmap,window1,gc,0,0,window1W,window1H,0,0);

   return n+1;
}
/*----------------------------------------------------------------------------*/

void marker_(float *x, float *y, float *w, float *h, int *cindex)
{
   int col;

   flush_xwi_();
   if (*cindex)
   {  xgcv.foreground = colind[1];    /* white */
      XChangeGC(dpy,gc,GCForeground,&xgcv);
      foreground_index = 1;
   }
   else
   {  xgcv.foreground = colind[8];    /* dark grey */
      XChangeGC(dpy,gc,GCForeground,&xgcv);
      foreground_index = 8;
   }
   XFillRectangle(dpy,window1,gc,
                 (int)*x,window1H-(int)*y-(int)*h,(int)*w,(int)*h);
   XFlush(dpy);
}
/*----------------------------------------------------------------------------*/

int getinter_(void)
/*
   increment count, if left mouse button or s key is pressed.
*/
{
   XEvent      event;
   static char param[] = "none";
   static int  count   = 0;
   int         j;

   if (focus != 1)
   {  XSetInputFocus(dpy,window1,RevertToParent,CurrentTime);
      focus = 1;
   }
   for (j=0; j<10; j++)   /* treat up to next 10 events */
      if (XCheckIfEvent(dpy,&event,predproc,param) )
         if (evnr == Button1 || evnr == 's' || evnr == 'S')
            count++;
   return count;
}
/*----------------------------------------------------------------------------*/

#ifdef TEST

/*------------------------------- test program -------------------------------*/

int main(void)
{
   int   i, jj, kk, ir=3, j=3, nn[4];
   int   ipar1, ipar2;
   char  str[100], mstr[40], answer[100];
   float x, y;
   float par1,par2,par3,par4;
   char  title[]  = "XLIB Test";
   char  titel[]  = "TITEL";
   char  prompt[] = "Enter> ";
   char  sample[] = "Sample";
   char  mnu[]    = "hallo urs hallo xyz hallo ttt hallo bob hallo yyy " ;

   nn[0] = 1; nn[1] = 3; nn[2] = 0; nn[3] = 0;

   par1 = 100.; par2 = 100.; par3 = 600.; par4 = 600.;
   inixwi_(title,&par1,&par2,&par3,&par4);

   for (jj=0; jj<2000; jj++)
   {  kk = getinter_();
      printf("%d\n",kk);
   }

   while(j--)
   {  erase_();
      par1=300.; par2=300.; par3=20.; par4=10.; ipar1=1;
      marker_(&par1,&par2,&par3,&par4,&ipar1);
      par1=250.; par2=550.; ipar1=2;
      text_out_(sample,&par1,&par2,&ipar1);
      par1=250.; par2=530.; ipar1=1;
      text_out_(sample,&par1,&par2,&ipar1);
      ipar1=1;
      set_col_l_(&ipar1);       /* white */
      ipar1=3;
      lined_(&ipar1);
      ipar1=0;
      linex_(&ipar1);           /* solid */
      par1=75.; par2=500.;
      gramov_(&par1,&par2);
      par1=300.; par2=100.;
      gradrw_(&par1,&par2);
      par1=525.; par2=500.;
      gradrw_(&par1,&par2);
      flush_xwi_();
      ipar1=1;
      lined_(&ipar1);
      ipar1=1;
      linex_(&ipar1);           /* dotted */
      par1=525.; par2=500.;
      gramov_(&par1,&par2);
      par1=50.; par2=225.;
      gradrw_(&par1,&par2);
      par1=575.; par2=225.;
      gradrw_(&par1,&par2);
      par1=75.; par2=500.;
      gradrw_(&par1,&par2);
      flush_xwi_();
      par1=300.; par2=300.; par3=20.; par4=10.; ipar1=0;
      marker_(&par1,&par3,&par3,&par4,&ipar1);

      vt220_pop_();
      printf("Input> ");
      scanf("%d",&i);
      vt220_push_();

      ipar1=1;
      set_col_(&ipar1);         /* white */
      par1=20.; par2=50.;
      getinp_(&par1,&par2,prompt,answer);
      par1=250.; par2=540.; par3=350.; par4=580.;
      eraser_(&par1,&par2,&par3,&par4);

      i = sscanf(answer,"%s %f %f",mstr,&x,&y);
      if ( (i == 3) && (strcmp(mstr,"MOUSE1") == 0) )
      {
         ipar1=10; ipar2=5;
         i = menu_(&x,&y,&ipar1,&ipar2,mnu,ipar1);
         printf("%d\n",i);
      }
      else if ( (i == 3) && (strcmp(mstr,"MOUSE2") == 0) )
      {
         ipar1=10; ipar2=5;
         ir = menu3_(&x,&y,&ipar1,&ipar2,mnu,titel,&ir,ipar1);
         printf("%d\n",ir);
      }
      else if ( (i == 3) && (strcmp(mstr,"MOUSE3") == 0) )
      {
         ipar1=10; ipar2=5;
         i = menu2_(&x,&y,&ipar1,&ipar2,mnu,nn,ipar1);
         printf("%d\n",i);
         for (i=0; i<4; i++)
            printf("%d ",nn[i]);
         printf("\n");
      }
      else if ( (i == 1) && (strcmp(mstr,"CONTROL_Z") == 0) )
      {  closer_();
         exit(-1);
      }
      else
         printf("%s\n",answer);

      vt220_pop_();
      printf("Input> ");
      scanf("%d",&i);
      vt220_push_();

   }
   closer_();
}

#endif

/************************************ EOF *************************************/
