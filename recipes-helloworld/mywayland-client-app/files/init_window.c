/* Compile with:
 * wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell.c
 * wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell.h
 * gcc -o test init_window.c xdg-shell.c -I. -lwayland-client -lwayland-server -lwayland-egl -lEGL -lGLESv2 -lm
 * need to thoroughly understand the interfaces - xdg-shell toplevel and surface, etc.
*/

#include <wayland-client-core.h>
#include <wayland-client.h>
#include <wayland-server.h>
#include <wayland-client-protocol.h>
#include <wayland-egl.h>
#include <linux/input.h> // For BTN_LEFT mouse button press
#include "xdg-shell.h"
#include "init_window.h"
#include "log.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <GLES2/gl2.h>

struct wl_compositor *compositor = NULL;
struct wl_surface *surface;
struct wl_egl_window *egl_window;
struct wl_region *region;

struct xdg_wm_base *XDGWMBase;
struct xdg_surface *XDGSurface;
struct xdg_toplevel *XDGToplevel;

struct wl_seat *current_seat = NULL;

struct _escontext ESContext = {
    .native_display = NULL,
    .window_width = 0,
    .window_height = 0,
    .native_window = 0,
    .display = NULL,
    .context = NULL,
    .surface = NULL
};

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

bool program_alive;
int32_t old_w, old_h;


// Function to get current time in milliseconds in order to implement color changing logic: 

unsigned long get_current_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/******************************/
/********XDG Toplevel**********/
/******************************/

static void xdg_toplevel_handle_configure(void *data,
    struct xdg_toplevel *xdg_toplevel, int32_t w, int32_t h,
    struct wl_array *states) {
    
    if (w == 0 && h == 0) return;

    if (old_w != w || old_h != h) {
        old_w = w;
        old_h = h;

        wl_egl_window_resize(ESContext.native_window, w, h, 0, 0);
        wl_surface_commit(surface);
    }
}

static void xdg_toplevel_handle_close(void *data,
    struct xdg_toplevel *xdg_toplevel) {
    program_alive = false;
}

struct xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = xdg_toplevel_handle_configure,
    .close = xdg_toplevel_handle_close,
};

/******************************/
/*********XDG Surface**********/
/******************************/

static void xdg_surface_configure(void *data, struct xdg_surface *xdg_surface,
    uint32_t serial) {
    xdg_surface_ack_configure(xdg_surface, serial);
}

const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

/******************************/
/******XDG Window Manager******/
/******************************/

static void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base,
    uint32_t serial) {
    xdg_wm_base_pong(xdg_wm_base, serial);
}

const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};


/*********************************/
/******Pointer Handling Code******/
/*********************************/

static void pointer_handle_enter(void *data, struct wl_pointer *pointer,
                                  uint32_t serial, struct wl_surface *surface,
                                  wl_fixed_t sx, wl_fixed_t sy) {
    // Handle pointer enter events if needed
}

static void pointer_handle_leave(void *data, struct wl_pointer *pointer,
                                  uint32_t serial, struct wl_surface *surface) {
    // Handle pointer leave events if needed
}

static void pointer_handle_motion(void *data, struct wl_pointer *pointer,
                                   uint32_t time, wl_fixed_t sx, wl_fixed_t sy) {
    // Optional: Handle pointer motion if needed
}

static void pointer_handle_button(void *data, struct wl_pointer *pointer,
                                   uint32_t serial, uint32_t time,
                                   uint32_t button, uint32_t state) {
    if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED) {
        if (current_seat) {
            xdg_toplevel_move(XDGToplevel, current_seat, serial); 
            // On a pointer button press, you can start a drag operation, and on pointer motion events, you will adjust the window position accordingly.
        } else {
            fprintf(stderr, "Seat is not set!\n");
        }
    }
}

static void pointer_handle_axis(void *data, struct wl_pointer *pointer,
                                 uint32_t time, uint32_t axis,
                                 wl_fixed_t value) {
    // Handle axis events if needed
}

// Pointer Listener:
const struct wl_pointer_listener pointer_listener = {
    .enter = pointer_handle_enter,
    .leave = pointer_handle_leave,
    .motion = pointer_handle_motion,
    .button = pointer_handle_button,
    .axis = pointer_handle_axis,
};

//**** Initialize window with EGL: *****
void CreateNativeWindow(char *title, int width, int height) {
    old_w = WINDOW_WIDTH;
    old_h = WINDOW_HEIGHT;

    region = wl_compositor_create_region(compositor);
    wl_region_add(region, 0, 0, width, height);
    wl_surface_set_opaque_region(surface, region);

    egl_window = wl_egl_window_create(surface, width, height);
    if (egl_window == EGL_NO_SURFACE) {
        LOG("No window! \n");
        exit(1);
    } else {
        LOG("Window created! \n");
    }

    ESContext.window_width = width;
    ESContext.window_height = height;
    ESContext.native_window = egl_window;
}

//**** Set up OpenGL ES context: ****
EGLBoolean CreateEGLContext() {
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint fbAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_NONE
    };
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

    EGLDisplay display = eglGetDisplay(ESContext.native_display);
    if (display == EGL_NO_DISPLAY) {
        LOG("No EGL Display \n");
        return EGL_FALSE;
    }

    if (!eglInitialize(display, &majorVersion, &minorVersion)) {
        LOG("No Initialisation \n");
        return EGL_FALSE;
    }

    if ((eglGetConfigs(display, NULL, 0, &numConfigs) != EGL_TRUE) || (numConfigs == 0)) {
        LOG("No configuration \n");
        return EGL_FALSE;
    }

    if ((eglChooseConfig(display, fbAttribs, &config, 1, &numConfigs) != EGL_TRUE) || (numConfigs != 1)) {
        LOG("No configuration \n");
        return EGL_FALSE;
    }

    surface = eglCreateWindowSurface(display, config, ESContext.native_window, NULL);
    if (surface == EGL_NO_SURFACE) {
        LOG("Failed to create EGL surface \n");
        return EGL_FALSE;
    }

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT) {
        LOG("Failed to create EGL context \n");
        return EGL_FALSE;
    }

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOG("Failed to make EGL context current \n");
        return EGL_FALSE;
    }

    ESContext.display = display;
    ESContext.surface = surface;
    ESContext.context = context;

    return EGL_TRUE;
}

EGLBoolean CreateWindowWithEGLContext(char *title, int width, int height) {
    CreateNativeWindow(title, width, height);
    return CreateEGLContext();
}

// **** Draw with Current Color - Render current frame: ****
void draw(float clear_color[3]) {
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

// **** Display rendered frame: 
void RefreshWindow() {
    eglSwapBuffers(ESContext.display, ESContext.surface);
}

/******************************/
/***********Registry***********/
/******************************/

static void global_registry_handler(void *data, struct wl_registry *registry,
                                     uint32_t id, const char *interface,
                                     uint32_t version) {
    LOG("Got a registry event for %s id %d\n", interface, id);
    
    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1); // Binds to necessary interfaces
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        XDGWMBase = wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(XDGWMBase, &xdg_wm_base_listener, NULL);
    } else if (strcmp(interface, "wl_seat") == 0) {
        struct wl_seat *seat = wl_registry_bind(registry, id, &wl_seat_interface, 1);
        current_seat = seat; // Store the seat for later use
        struct wl_pointer *pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(pointer, &pointer_listener, NULL);
    }
}

static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id) {
    LOG("Got a registry losing event for %d\n", id);
}

const struct wl_registry_listener listener = {
    global_registry_handler,
    global_registry_remover
};

static void get_server_references() {
    struct wl_display *display = wl_display_connect(NULL); 
    if (display == NULL) {
        LOG("Can't connect to wayland display !?\n");
        exit(1);
    }
    LOG("Got a display !");

    struct wl_registry *wl_registry = wl_display_get_registry(display); // To access global objects
    wl_registry_add_listener(wl_registry, &listener, NULL); // To add registry listener to handle global events
    wl_display_dispatch(display); // To process pending events
    wl_display_roundtrip(display);

    if (compositor == NULL || XDGWMBase == NULL) {
        LOG("No compositor!? No XDG shell surface!! There's NOTHING in here! \n");
        exit(1);
    } else {
        LOG("Okay, we got a compositor and a shell... That's something! \n");
        ESContext.native_display = display;
    }
}

void destroy_window() {
    eglDestroySurface(ESContext.display, ESContext.surface);
    wl_egl_window_destroy(ESContext.native_window);
    xdg_toplevel_destroy(XDGToplevel);
    xdg_surface_destroy(XDGSurface);
    wl_surface_destroy(surface);
    eglDestroyContext(ESContext.display, ESContext.context);
}

int main() {
    get_server_references();

    surface = wl_compositor_create_surface(compositor);
    if (surface == NULL) {
        LOG("No Compositor surface! \n");
        exit(1);
    } else {
        LOG("Got a compositor surface! \n");
    }

    XDGSurface = xdg_wm_base_get_xdg_surface(XDGWMBase, surface);
    xdg_surface_add_listener(XDGSurface, &xdg_surface_listener, NULL);

    XDGToplevel = xdg_surface_get_toplevel(XDGSurface);
    xdg_toplevel_set_title(XDGToplevel, "Wayland EGL Example");
    xdg_toplevel_add_listener(XDGToplevel, &xdg_toplevel_listener, NULL);

    wl_surface_commit(surface);
    CreateWindowWithEGLContext("Nya", 1280, 720);
    
    unsigned long last_color_change = get_current_time_ms();
    float clear_color[3] = {0.0, 0.0, 0.0}; // Initial color

    program_alive = true;

    while (program_alive) {
        unsigned long current_time = get_current_time_ms();

        if (current_time - last_color_change >= 1000) {
            // Update color to a new random value
            clear_color[0] = (GLfloat)(rand() % 128) / 128.0; // Red
            clear_color[1] = (GLfloat)(rand() % 128) / 128.0; // Green
            clear_color[2] = (GLfloat)(rand() % 128) / 128.0; // Blue
            last_color_change = current_time;
        }

        // Process Wayland events
        wl_display_dispatch_pending(ESContext.native_display);

        // Render
        draw(clear_color);
        RefreshWindow();
    }

    destroy_window();
    wl_display_disconnect(ESContext.native_display);
    LOG("Display disconnected !\n");

    exit(0);
}

