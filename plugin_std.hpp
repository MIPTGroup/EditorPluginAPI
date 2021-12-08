#ifndef PLUGIN_STD_HPP
#define PLUGIN_STD_HPP


#include "types_std.h"


// this function is only defined in plugin. call it to get PluginInterface to interact with plugin
// make sure you wrap it into extern C section to avoid mangling
// const PluginInterface *get_plugin_interface();

#if __cplusplus >= 201703L

constexpr char PGET_INTERFACE_FUNC[] = "get_plugin_interface";
constexpr uint32_t PSTD_VERSION = 0;

constexpr char PST_TEXT_LINE[]    = "pst_text_line";
constexpr char PST_SLIDER_1D[]    = "pst_slider_1d";
constexpr char PST_SLIDER_2D[]    = "pst_slider_2d";
constexpr char PST_COLOR_PICKER[] = "pst_color_picker";

constexpr char PEXT_STD[] = "std";

#else

#define PGET_INTERFACE_FUNC "get_plugin_interface"
#define PSTD_VERSION 0

#define PST_TEXT_LINE    "pst_text_line"
#define PST_SLIDER_1D    "pst_slider_1d"
#define PST_SLIDER_2D    "pst_slider_2d"
#define PST_COLOR_PICKER "pst_color_picker"

#define PEXT_STD  "std"

#endif


namespace P {


struct PluginInterface;

struct PluginInfo {
    uint32_t std_version;
    void *reserved;

    const PluginInterface *interface;

    const char *name;
    const char *version;
    const char *author;
    const char *description;

    PluginType type;
};

struct AppInterface;
struct PluginInterface {
    uint32_t std_version;
    void *reserved;

    struct {
        // enables specified extension
        bool  (*enable)(const char *name);

        // returns given function, if it is implemented in the specified (enabled) extension
        void *(*get_func)(const char *extension, const char *func);
    } extensions;

    struct {
        const PluginInfo *(*get_info)();
        Status (*init)(const AppInterface*);
        Status (*deinit)();
        void (*dump)();

        void (*on_tick)(double dt);
        void (*on_settings_update)();

        PreviewLayerPolicy (*get_flush_policy)();
    } general;

    struct {
        void (*apply)();
    } effect;

    struct {
        void (*on_press)(Vec2f mouse_pos);
        void (*on_release)(Vec2f mouse_pos);
        void (*on_move)(Vec2f mouse_old_pos, Vec2f mouse_new_pos);
    } tool;
};

struct PAppInterface {
    uint32_t std_version;
    void *reserved;

    struct {
        // enables specified extension
        bool  (*enable)(const char *name);

        // returns given function, if it is implemented in the specified (enabled) extension
        void *(*get_func)(const char *extension, const char *func);
    } extensions;

    struct {
        int feature_level;

        void (*log)(const char *fmt, ...);
        double (*get_absolute_time)();

        void (*release_pixels)(RGBA *pixels);

        RGBA (*get_color)();
        float (*get_size)();
    } general;

    struct {
        RGBA *(*get_pixels)();
        void (*get_size)(size_t *width, size_t *height);
    } target;

    struct {
        void (*circle)(Vec2f position, float radius, RGBA color, const RenderMode *render_mode);
        void (*line)(Vec2f start, Vec2f end, RGBA color, const RenderMode *render_mode);
        void (*triangle)(Vec2f p1, Vec2f p2, Vec2f p3, RGBA color, const RenderMode *render_mode);
        void (*rectangle)(Vec2f p1, Vec2f p2, RGBA color, const RenderMode *render_mode);

        void (*pixels)(Vec2f position, const RGBA *data, size_t width, size_t height, const RenderMode *render_mode);
    } render;

    // set everything to nullptr here if you don't support shaders
    struct {
        void (*apply)(const RenderMode *render_mode);

        void *(*compile)(const char *code, ShaderType type);
        void  (*release)(void *);

        void (*set_uniform_int)      (void *shader, const char *name, int  val);
        void (*set_uniform_int_arr)  (void *shader, const char *name, int *val, size_t cnt);

        void (*set_uniform_float)    (void *shader, const char *name, float  val);
        void (*set_uniform_float_arr)(void *shader, const char *name, float *val, size_t cnt);
    } shader;
};

}

#endif
