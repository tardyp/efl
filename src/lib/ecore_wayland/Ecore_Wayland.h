#ifndef _ECORE_WAYLAND_H_
# define _ECORE_WAYLAND_H_

/*
 * Wayland supoprt is considered experimental as wayland itself is still
 * unstable and liable to change core protocol. If you use this api, it is
 * possible it will break in future, until this notice is removed.
 */

# include <Eina.h>
# include <Ecore.h>
# include <wayland-client.h>
# include <wayland-cursor.h>
# include <xkbcommon/xkbcommon.h>

# ifdef EAPI
#  undef EAPI
# endif

# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Ecore_Wl_Display Ecore_Wl_Display;
typedef struct _Ecore_Wl_Output Ecore_Wl_Output;
typedef struct _Ecore_Wl_Input Ecore_Wl_Input;
typedef struct _Ecore_Wl_Global Ecore_Wl_Global; /** @since 1.7.6 */

# ifndef _ECORE_WAYLAND_WINDOW_PREDEF
typedef struct _Ecore_Wl_Window Ecore_Wl_Window;
# endif
typedef struct _Ecore_Wl_Dnd Ecore_Wl_Dnd; /** @since 1.7 */
typedef struct _Ecore_Wl_Dnd_Source Ecore_Wl_Dnd_Source;
typedef struct _Ecore_Wl_Dnd_Target Ecore_Wl_Dnd_Target;

typedef struct _Ecore_Wl_Event_Mouse_In Ecore_Wl_Event_Mouse_In;
typedef struct _Ecore_Wl_Event_Mouse_Out Ecore_Wl_Event_Mouse_Out;
typedef struct _Ecore_Wl_Event_Focus_In Ecore_Wl_Event_Focus_In;
typedef struct _Ecore_Wl_Event_Focus_Out Ecore_Wl_Event_Focus_Out;
typedef struct _Ecore_Wl_Event_Window_Configure Ecore_Wl_Event_Window_Configure;
typedef struct _Ecore_Wl_Event_Dnd_Enter Ecore_Wl_Event_Dnd_Enter;
typedef struct _Ecore_Wl_Event_Dnd_Position Ecore_Wl_Event_Dnd_Position;
typedef struct _Ecore_Wl_Event_Dnd_Leave Ecore_Wl_Event_Dnd_Leave;
typedef struct _Ecore_Wl_Event_Dnd_Drop Ecore_Wl_Event_Dnd_Drop;
typedef struct _Ecore_Wl_Event_Data_Source_Send Ecore_Wl_Event_Data_Source_Send; /** @since 1.7 */
typedef struct _Ecore_Wl_Event_Selection_Data_Ready Ecore_Wl_Event_Selection_Data_Ready; /** @since 1.7 */
typedef struct _Ecore_Wl_Event_Interfaces_Bound Ecore_Wl_Event_Interfaces_Bound;

enum _Ecore_Wl_Window_Type
{
   ECORE_WL_WINDOW_TYPE_NONE,
   ECORE_WL_WINDOW_TYPE_TOPLEVEL,
   ECORE_WL_WINDOW_TYPE_FULLSCREEN,
   ECORE_WL_WINDOW_TYPE_MAXIMIZED,
   ECORE_WL_WINDOW_TYPE_TRANSIENT,
   ECORE_WL_WINDOW_TYPE_MENU,
   ECORE_WL_WINDOW_TYPE_CUSTOM
};

enum _Ecore_Wl_Window_Buffer_Type
{
   ECORE_WL_WINDOW_BUFFER_TYPE_EGL_WINDOW,
   ECORE_WL_WINDOW_BUFFER_TYPE_EGL_IMAGE,
   ECORE_WL_WINDOW_BUFFER_TYPE_SHM
};

typedef enum _Ecore_Wl_Window_Type Ecore_Wl_Window_Type;
typedef enum _Ecore_Wl_Window_Buffer_Type Ecore_Wl_Window_Buffer_Type;

/** @since 1.7.6 */
struct _Ecore_Wl_Global
{
   unsigned int id;
   char *interface;
   unsigned int version;
   struct wl_list link;
};

struct _Ecore_Wl_Display
{
   struct 
     {
        struct wl_display *display;
        struct wl_registry *registry;
        struct wl_compositor *compositor;
        struct wl_shell *shell;
        struct wl_shell *desktop_shell;
        struct wl_shm *shm;
        struct wl_data_device_manager *data_device_manager;
     } wl;

   int fd;
   unsigned int mask;
   unsigned int serial;
   Ecore_Fd_Handler *fd_hdl;
   Ecore_Idle_Enterer *idle_enterer;

   struct wl_list inputs;
   struct wl_list outputs;
   struct wl_list globals; /** @since 1.7.6 */

   struct
     {
        struct xkb_context *context;
     } xkb;

   struct wl_cursor_theme *cursor_theme;

   Ecore_Wl_Output *output;
   Ecore_Wl_Input *input;

   void (*output_configure)(Ecore_Wl_Output *output, void *data);
   void *data;
};

struct _Ecore_Wl_Output
{
   Ecore_Wl_Display *display;
   struct wl_output *output;
   Eina_Rectangle allocation;
   int mw, mh;
   struct wl_list link;

   void (*destroy) (Ecore_Wl_Output *output, void *data);
   void *data;
};

struct _Ecore_Wl_Input
{
   Ecore_Wl_Display *display;
   struct wl_seat *seat;
   struct wl_pointer *pointer;
   struct wl_keyboard *keyboard;
   struct wl_touch *touch;

   const char *cursor_name;
   struct wl_surface *cursor_surface;
   struct wl_callback *cursor_frame_cb;

   struct wl_data_device *data_device;

   Ecore_Wl_Window *pointer_focus;
   Ecore_Wl_Window *keyboard_focus;

   unsigned int button;
   unsigned int timestamp;
   unsigned int modifiers;
   unsigned int pointer_enter_serial;
   int sx, sy;

   struct wl_list link;

   Ecore_Wl_Window *grab;
   unsigned int grab_button;

   Ecore_Wl_Dnd_Source *drag_source;
   Ecore_Wl_Dnd_Source *selection_source;
   Ecore_Wl_Dnd *dnd; /** @since 1.7 */

   struct
     {
        struct xkb_keymap *keymap;
        struct xkb_state *state;
        xkb_mod_mask_t control_mask;
        xkb_mod_mask_t alt_mask;
        xkb_mod_mask_t shift_mask;
     } xkb;

   struct 
     {
        Ecore_Fd_Handler *hdlr;
        int timerfd;
        unsigned int sym, key, time;
     } repeat;
};

struct _Ecore_Wl_Window
{
   Ecore_Wl_Display *display;
   Ecore_Wl_Window *parent;

   struct wl_surface *surface;
   struct wl_shell_surface *shell_surface;

   struct 
     {
        struct wl_region *input, *opaque;
     } region;

   int id;
   int x, y;
   int edges;

   Eina_Rectangle allocation, pending_allocation;
   Eina_Rectangle saved_allocation, server_allocation;

   /* Eina_Bool redraw_scheduled : 1; */
   /* Eina_Bool resize_scheduled : 1; */
   Eina_Bool alpha : 1;
   Eina_Bool transparent : 1;
   Eina_Bool moving : 1;
   Eina_Bool resizing : 1;

   Ecore_Wl_Window_Type type;
   Ecore_Wl_Window_Buffer_Type buffer_type;

   Ecore_Wl_Input *pointer_device;
   Ecore_Wl_Input *keyboard_device;

   /* FIXME: Shouldn't these attributes be private to the Ecore_Wl_Window? */
   Eina_Bool frame_pending;
   struct wl_callback *frame_callback;
   void (*state_changed_cb)(void *);
   void *state_changed_cb_data;
   Ecore_Job *state_changed_job;
   /* FIXME: Ideally we should record the cursor name for this window 
    * so we can compare and avoid unnecessary cursor set calls to wayland */

   void *data;
};

struct _Ecore_Wl_Event_Mouse_In
{
   int modifiers;
   int x, y;
   struct 
     {
        int x, y;
     } root;
   unsigned int window;
   unsigned int event_window;
   unsigned int root_window;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Mouse_Out
{
   int modifiers;
   int x, y;
   struct 
     {
        int x, y;
     } root;
   unsigned int window;
   unsigned int event_window;
   unsigned int root_window;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Focus_In
{
   unsigned int win;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Focus_Out
{
   unsigned int win;
   unsigned int timestamp;
};

struct _Ecore_Wl_Event_Window_Configure
{
   unsigned int win;
   unsigned int event_win;
   int x, y, w, h;
};

struct _Ecore_Wl_Event_Dnd_Enter
{
   unsigned int win, source;
   char **types;
   int num_types;
   struct 
     {
        int x, y;
     } position;
};

struct _Ecore_Wl_Event_Dnd_Position
{
   unsigned int win, source;
   struct 
     {
        int x, y;
     } position;
};

struct _Ecore_Wl_Event_Dnd_Leave
{
   unsigned int win, source;
};

struct _Ecore_Wl_Event_Dnd_Drop
{
   unsigned int win, source;
   struct 
     {
        int x, y;
     } position;
};

/** @since 1.7 */
struct _Ecore_Wl_Event_Data_Source_Send
{
   char *type;
   int fd;
};

/** @since 1.7 */
struct _Ecore_Wl_Event_Selection_Data_Ready
{
   char *data;
   int len;
   Eina_Bool done;
};

struct _Ecore_Wl_Event_Interfaces_Bound
{
   Eina_Bool compositor : 1;
   Eina_Bool shm : 1;
   Eina_Bool shell : 1;
};

/**
 * @file
 * @brief Ecore functions for dealing with the Wayland window system
 *
 * @defgroup Ecore_Wl_Group Ecore_Wayland - Wayland integration
 * @ingroup Ecore

 * Ecore_Wl provides a wrapper and convenience functions for using the 
 * Wayland window system. Function groups for this part of the library 
 * include the following:
 * 
 * @li @ref Ecore_Wl_Init_Group
 * @li @ref Ecore_Wl_Display_Group
 * @li @ref Ecore_Wl_Flush_Group
 * @li @ref Ecore_Wl_Window_Group
 */

EAPI extern int ECORE_WL_EVENT_MOUSE_IN;
EAPI extern int ECORE_WL_EVENT_MOUSE_OUT;
EAPI extern int ECORE_WL_EVENT_FOCUS_IN;
EAPI extern int ECORE_WL_EVENT_FOCUS_OUT;
EAPI extern int ECORE_WL_EVENT_WINDOW_CONFIGURE;
EAPI extern int ECORE_WL_EVENT_DND_ENTER;
EAPI extern int ECORE_WL_EVENT_DND_POSITION;
EAPI extern int ECORE_WL_EVENT_DND_LEAVE;
EAPI extern int ECORE_WL_EVENT_DND_DROP;
EAPI extern int ECORE_WL_EVENT_DATA_SOURCE_TARGET; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_DATA_SOURCE_SEND; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_DATA_SOURCE_CANCELLED; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_SELECTION_DATA_READY; /** @since 1.7 */
EAPI extern int ECORE_WL_EVENT_INTERFACES_BOUND;

/**
 * @defgroup Ecore_Wl_Init_Group Wayland Library Init and Shutdown Functions
 * @ingroup Ecore_Wl_Group
 *
 * Functions that start and shutdown the Ecore Wayland Library.
 */

/**
 * Initialize the Wayland display connection to the given display.
 *
 * @param   name Display target name. if @c NULL, the default display is
 *          assumed.
 * @return  The number of times the library has been initialized without being
 *          shut down. 0 is returned if an error occurs.
 *
 * @ingroup Ecore_Wl_Init_Group
 */
EAPI int ecore_wl_init(const char *name);

/**
 * Shuts down the Ecore Wayland Library
 *
 * In shutting down the library, the Wayland display connection is terminated
 * and any event handlers for it are removed.
 *
 * @return  The number of times the library has been initialized without
 *          being shut down.
 *
 * @ingroup Ecore_Wl_Init_Group
 */
EAPI int ecore_wl_shutdown(void);

/**
 * @defgroup Ecore_Wl_Flush_Group Wayland Synchronization Functions
 * @ingroup Ecore_Wl_Group
 *
 * Functions that ensure that all commands which have been issued by the
 * Ecore Wayland library have been sent to the server.
 */

/**
 * Sends all Wayland commands to the Wayland Display.
 *
 * @ingroup Ecore_Wl_Flush_Group
 * @since 1.2
 */
EAPI void ecore_wl_flush(void);

/**
 * Flushes the command buffer and waits until all requests have been
 * processed by the server.
 *
 * @ingroup Ecore_Wl_Flush_Group
 * @since 1.2
 */
EAPI void ecore_wl_sync(void);

/**
 * @defgroup Ecore_Wl_Display_Group Wayland Display Functions
 * @ingroup Ecore_Wl_Group
 *
 * Functions that set and retrieve various information about the Wayland Display.
 */

/**
 * Retrieves the Wayland Shm Interface used for the current Wayland connection.
 *
 * @return The current wayland shm interface
 *
 * @ingroup Ecore_Wl_Display_Group
 * @since 1.2
 */
EAPI struct wl_shm *ecore_wl_shm_get(void);

/**
 * Retrieves the Wayland Display Interface used for the current Wayland connection.
 *
 * @return The current wayland display interface
 *
 * @ingroup Ecore_Wl_Display_Group
 * @since 1.2
 */
EAPI struct wl_display *ecore_wl_display_get(void);

/**
 * Retrieves the size of the current screen.
 *
 * @param w where to return the width. May be NULL. Returns 0 on error.
 * @param h where to return the height. May be NULL. Returns 0 on error.
 *
 * @ingroup Ecore_Wl_Display_Group
 * @since 1.2
 */
EAPI void ecore_wl_screen_size_get(int *w, int *h);
EAPI void ecore_wl_pointer_xy_get(int *x, int *y);

/**
 * Return the screen DPI
 *
 * This is a simplistic call to get DPI. It does not account for differing
 * DPI in the x and y axes nor does it account for multihead or xinerama and
 * xrandr where different parts of the screen may have different DPI etc.
 *
 * @return the general screen DPI (dots/pixels per inch).
 *
 * @since 1.2
 */
EAPI int ecore_wl_dpi_get(void);
EAPI void ecore_wl_display_iterate(void);

/**
 * Retrieves the requested cursor from the cursor theme
 *
 * @param cursor_name The desired cursor name to be looked up in the theme
 * @return the cursor or NULL if the cursor cannot be found
 *
 * @since 1.2
 */
EAPI struct wl_cursor *ecore_wl_cursor_get(const char *cursor_name);

EAPI void ecore_wl_input_grab(Ecore_Wl_Input *input, Ecore_Wl_Window *win, unsigned int button);
EAPI void ecore_wl_input_ungrab(Ecore_Wl_Input *input);
EAPI void ecore_wl_input_pointer_set(Ecore_Wl_Input *input, struct wl_surface *surface, int hot_x, int hot_y);
EAPI void ecore_wl_input_cursor_from_name_set(Ecore_Wl_Input *input, const char *cursor_name);
EAPI void ecore_wl_input_cursor_default_restore(Ecore_Wl_Input *input);

EAPI struct wl_list *ecore_wl_outputs_get(void);

/**
 * Retrieves the Wayland Globals Interface list used for the current Wayland connection.
 *
 * @return The current wayland globals interface list
 *
 * @ingroup Ecore_Wl_Display_Group
 * @since 1.7.6
 */
EAPI struct wl_list *ecore_wl_globals_get(void);

/**
 * Retrieves the Wayland Registry used for the current Wayland connection.
 *
 * @return The current wayland registry
 *
 * @ingroup Ecore_Wl_Display_Group
 * @since 1.7.6
 */
EAPI struct wl_registry *ecore_wl_registry_get(void);

/**
 * @defgroup Ecore_Wl_Window_Group Wayland Window functions.
 * @ingroup Ecore_Wl_Group
 *
 * Functions that can be used to create a Wayland window.
 */

/**
 * Creates a new window
 * 
 * @param parent The parent window to use. If @p parent is @c 0, the root window 
 *               of the default display is used.
 * @param x      X Position
 * @param y      Y position
 * @param w      Width
 * @param h      Height
 * @param buffer_type The type of the buffer to be used to create a new Ecore_Wl_Window.
 * 
 * @return The new window
 * 
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.2
 */
EAPI Ecore_Wl_Window *ecore_wl_window_new(Ecore_Wl_Window *parent, int x, int y, int w, int h, int buffer_type);

/**
 * Deletes the given window
 * 
 * @param win The given window
 * 
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.2
 */
EAPI void ecore_wl_window_free(Ecore_Wl_Window *win);

/**
 * Signals for Wayland to initiate a window move.
 * 
 * The position requested (@p x, @p y) is not honored by Wayland because 
 * Wayland does not allow specific window placement to be set.
 * 
 * @param win The window to move.
 * @param x   X Position
 * @param y   Y Position
 * 
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.2
 */
EAPI void ecore_wl_window_move(Ecore_Wl_Window *win, int x, int y);

/**
 * Signals for Wayland to initiate a window resize.
 * 
 * The size requested (@p w, @p h) is not honored by Wayland because 
 * Wayland does not allow specific window sizes to be set.
 * 
 * @param win      The window to resize.
 * @param w        Width
 * @param h        Height
 * @param location The edge of the window from where the resize should start.
 * 
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.2
 */
EAPI void ecore_wl_window_resize(Ecore_Wl_Window *win, int w, int h, int location);
EAPI void ecore_wl_window_damage(Ecore_Wl_Window *win, int x, int y, int w, int h);
EAPI void ecore_wl_window_buffer_attach(Ecore_Wl_Window *win, struct wl_buffer *buffer, int x, int y);

/**
 * Shows a window
 * 
 * Synonymous to "mapping" a window in Wayland System terminology.
 * 
 * @param win The window to show.
 * 
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.2
 */
EAPI void ecore_wl_window_show(Ecore_Wl_Window *win);

/**
 * Hides a window
 * 
 * Synonymous to "unmapping" a window in Wayland System terminology.
 * 
 * @param win The window to hide.
 * 
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.2
 */
EAPI void ecore_wl_window_hide(Ecore_Wl_Window *win);

/**
 * Raises a window
 * 
 * @param win The window to raise.
 * 
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.2
 */
EAPI void ecore_wl_window_raise(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_maximized_set(Ecore_Wl_Window *win, Eina_Bool maximized);
EAPI void ecore_wl_window_fullscreen_set(Ecore_Wl_Window *win, Eina_Bool fullscreen);
EAPI void ecore_wl_window_transparent_set(Ecore_Wl_Window *win, Eina_Bool transparent);
EAPI void ecore_wl_window_update_size(Ecore_Wl_Window *win, int w, int h);
EAPI void ecore_wl_window_update_location(Ecore_Wl_Window *win, int x, int y);
EAPI struct wl_surface *ecore_wl_window_surface_get(Ecore_Wl_Window *win);
EAPI struct wl_shell_surface *ecore_wl_window_shell_surface_get(Ecore_Wl_Window *win);
EAPI Ecore_Wl_Window *ecore_wl_window_find(unsigned int id);
EAPI void ecore_wl_window_type_set(Ecore_Wl_Window *win, Ecore_Wl_Window_Type type);
EAPI void ecore_wl_window_pointer_set(Ecore_Wl_Window *win, struct wl_surface *surface, int hot_x, int hot_y);
EAPI void ecore_wl_window_cursor_from_name_set(Ecore_Wl_Window *win, const char *cursor_name);
EAPI void ecore_wl_window_cursor_default_restore(Ecore_Wl_Window *win);
EAPI void ecore_wl_window_parent_set(Ecore_Wl_Window *win, Ecore_Wl_Window *parent);
EAPI void ecore_wl_window_state_changed_cb_set(Ecore_Wl_Window *win, void (*cb)(void *), void *cb_data);

/** @since 1.7 */
EAPI Eina_Bool ecore_wl_dnd_set_selection(Ecore_Wl_Dnd *dnd, const char **types_offered);
EAPI Eina_Bool ecore_wl_dnd_get_selection(Ecore_Wl_Dnd *dnd, const char *type);
EAPI Ecore_Wl_Dnd *ecore_wl_dnd_get();
EAPI Eina_Bool ecore_wl_dnd_start_drag();
EAPI Eina_Bool ecore_wl_dnd_selection_has_owner(Ecore_Wl_Dnd *dnd);

/**
 * Returns a wl_surface with no association to any wl_shell_surface.
 *
 * @param win The window which will use this newly created surface.
 *
 * @ingroup Ecore_Wl_Window_Group
 * @since 1.8
 */
EAPI struct wl_surface *ecore_wl_window_surface_create(Ecore_Wl_Window *win);

#ifdef __cplusplus
}
#endif

#endif
