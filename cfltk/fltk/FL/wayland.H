//
// Wayland/X11 hybrid platform header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2022 by Bill Spitzak and others.
// Modified 2026-10-01 by the HeroineOS project (https://github.com/HeroineOS/fltk-sys):
// wlr-layer-shell support, transparent windows, anchored popups.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

#if !defined(FL_PLATFORM_H)
#  error "Never use <FL/wayland.H> directly; include <FL/platform.H> instead."
#endif // !FL_PLATFORM_H

/** \file
 \anchor wayland_H Definitions of functions specific to the Wayland platform.
*/

// ***********  for Wayland component ***********

typedef struct _cairo cairo_t;

/** Returns the Wayland display in use */
extern FL_EXPORT struct wl_display *fl_wl_display();
/** Returns the wl_surface associated to a shown window */
extern FL_EXPORT struct wl_surface *fl_wl_surface(struct wld_window *xid);
/** Returns a platform-specific reference associated to a shown window */
extern FL_EXPORT struct wld_window *fl_wl_xid(const Fl_Window *win);
/** Returns the Fl_Window corresponding to a given the platform-specific window reference */
extern FL_EXPORT Fl_Window *fl_wl_find(struct wld_window *);
/** Returns the cairo context associated to the current window or Fl_Image_Surface */
extern FL_EXPORT cairo_t *fl_wl_gc();
/** Returns the wl_compositor of the current Wayland session.
 This allows, for example, to create a wl_surface with
 \code
 struct wl_surface *my_wl_surface = wl_compositor_create_surface(fl_wl_compositor());
 \endcode
 */
extern FL_EXPORT struct wl_compositor *fl_wl_compositor();
/** Returns the current buffer scaling factor for \p window. */
extern FL_EXPORT int fl_wl_buffer_scale(Fl_Window *window);
typedef void *EGLContext;
/** Returns the EGLContext corresponding to the given GLContext */
extern FL_EXPORT EGLContext fl_wl_glcontext(GLContext rc);

/** Layers of the wlr-layer-shell protocol, bottom to top.
 \see fl_wl_layer_window() */
enum Fl_Wl_Layer {
  FL_WL_LAYER_BACKGROUND = 0, ///< wallpapers
  FL_WL_LAYER_BOTTOM = 1,     ///< below windows: desktop widgets
  FL_WL_LAYER_TOP = 2,        ///< above windows: panels, docks
  FL_WL_LAYER_OVERLAY = 3     ///< above everything, even fullscreen windows: OSDs, lock screens
};
/** Screen edges a layer-shell window is anchored to; combine with '|'.
 \see fl_wl_layer_window() */
enum Fl_Wl_Anchor {
  FL_WL_ANCHOR_TOP = 1,
  FL_WL_ANCHOR_BOTTOM = 2,
  FL_WL_ANCHOR_LEFT = 4,
  FL_WL_ANCHOR_RIGHT = 8
};
/** Whether a layer-shell window receives keyboard input.
 \see fl_wl_layer_window() */
enum Fl_Wl_Keyboard {
  FL_WL_KEYBOARD_NONE = 0,      ///< never (panels that are only clicked)
  FL_WL_KEYBOARD_EXCLUSIVE = 1, ///< always, while the window is shown (launchers, lock screens)
  FL_WL_KEYBOARD_ON_DEMAND = 2  ///< like a regular window: when clicked or focused
};
extern FL_EXPORT int fl_wl_has_layer_shell();
extern FL_EXPORT void fl_wl_layer_window(Fl_Window *win, enum Fl_Wl_Layer layer, int anchor,
                                         int exclusive_zone, enum Fl_Wl_Keyboard keyboard,
                                         int screen, const char *name_space);
extern FL_EXPORT void fl_wl_layer_margins(Fl_Window *win, int top, int right, int bottom,
                                          int left);
extern FL_EXPORT void fl_wl_transparent(Fl_Window *win);
extern FL_EXPORT void fl_wl_clear_rect(int x, int y, int w, int h);
extern FL_EXPORT void fl_wl_popup(Fl_Window *win, Fl_Window *parent, int x, int y, int w,
                                  int h);

#ifndef FL_DOXYGEN

#  ifdef FLTK_USE_X11
//  *********** for X11 component ***********
#    include "x11.H"
#  else
      typedef struct wld_window *Window;
#  endif

#endif // FL_DOXYGEN
