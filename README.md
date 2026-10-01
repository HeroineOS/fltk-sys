# fltk-sys (HeroineOS fork)

This is [fltk-sys 1.5.23](https://crates.io/crates/fltk-sys/1.5.23) from
[fltk-rs](https://github.com/fltk-rs/fltk-rs) with two additions to FLTK's Wayland backend:
**touchscreen input** (FLTK 1.4 ignores Wayland touch events; the first finger now acts
as the left mouse button, like on X11), and support for the
**wlr-layer-shell** Wayland protocol, which desktop components (panels, docks, desktop
widgets, notifications, wallpapers) use to sit on dedicated layers anchored to screen
edges. Upstream FLTK declined it as Wayland-specific
([fltk/fltk#593](https://github.com/fltk/fltk/issues/593)), so it lives here.

Everything else is unchanged: same version number, same API plus three functions, so
`fltk` from crates.io and other fltk crates build against it as usual. Apps opt in with:

```toml
[patch.crates-io]
fltk-sys = { git = "https://github.com/HeroineOS/fltk-sys" }
```

Apps that need layer-shell or touchscreen input on Wayland should do this.

### What's added

- FLTK (`cfltk/fltk`, Wayland backend only): `fl_wl_has_layer_shell()`,
  `fl_wl_layer_window()`, `fl_wl_layer_margins()` in `FL/wayland.H` (documented
  there). A window marked before `show()` becomes a layer surface when the compositor
  supports the protocol, and a regular window otherwise. Its menus and tooltips work
  (they become popups of the layer surface). Borderless windows also get an
  `app_id`, like decorated ones already did.
- FLTK: touch events (`wl_touch`) are processed as mouse button 1: push, drag, release
  for the first finger; other fingers are ignored; a touch sequence cancelled by the
  compositor releases the button without a click.
- cfltk: `Fl_wl_has_layer_shell`, `Fl_Window_wl_layer_window`, `Fl_Window_wl_layer_margins`
  (no-ops returning 0 without the Wayland backend).
- Rust (`src/window.rs`): bindings for those three functions.
- `cfltk/fltk/src/drivers/Wayland/wlr-layer-shell-unstable-v1.xml`, the protocol
  (© 2017 Drew DeVault, see its license header), as wayland-protocols doesn't ship it.

Modified FLTK files carry a notice, as FLTK's license (LGPL with exceptions) asks.
Requires the `use-wayland` feature and the usual Wayland build dependencies.

The original README follows.

---

# fltk-sys

Raw bindings for FLTK. These are generated using bindgen on the cfltk headers.

## Usage
```toml
[dependencies]
fltk-sys = "1.5"
```

Example code:
```rust,no_run
use fltk_sys::*;
use std::os::raw::*;

unsafe extern "C" fn cb(_wid: *mut button::Fl_Widget, data: *mut c_void) {
    let frame = data as *mut frame::Fl_Box;
    frame::Fl_Box_set_label(frame, "Hello World\0".as_ptr() as *const _);
}

fn main() {
    unsafe {
        fl::Fl_init_all();
        image::Fl_register_images();
        fl::Fl_lock();
        let win = window::Fl_Window_new(100, 100, 400, 300, "Window\0".as_ptr() as *const _);
        let frame = frame::Fl_Box_new(0, 0, 400, 200, std::ptr::null());
        let but = button::Fl_Button_new(160, 220, 80, 40, "Click\0".as_ptr() as *const _);
        window::Fl_Window_end(win);
        window::Fl_Window_show(win);

        button::Fl_Button_set_callback(but, Some(cb), frame as *mut _);

        fl::Fl_run();
    }
}
```

## Dependencies
CMake > 3.14, git and a C++17 compiler. The dev dependencies are basically the same as for [fltk-rs](https://github.com/fltk-rs/fltk-rs#dependencies).

## Why you might want to use fltk-sys directly
- If you need an abi stable cdylib that you can call into (as a plugin system for example).
- To create your own wrapper around certain elements if you don't need the whole fltk crate.
- fltk-sys, although memory and thread unsafe, is panic-safe.
- You need a no-std gui library, in such case, you can replace the `std::` prefix with the `libc` via bindgen (requires adding libc as a dependency).
- Wrapping a 3rd-party widget like in [fltk-flow](https://github.com/fltk-rs/fltk-flow).