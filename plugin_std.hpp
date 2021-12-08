#ifndef PLUGIN_STD_HPP
#define PLUGIN_STD_HPP


#include <string>

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

    // enables specified extension
    virtual bool enable(const char *name) = 0;

    // returns given function, if it is implemented in the specified (enabled) extension
    virtual void *get_func(const char *extension, const char *func) = 0;

    virtual const PluginInfo *get_info() = 0;
    virtual Status init(const AppInterface*) = 0;
    virtual Status deinit() = 0;
    virtual void dump() = 0;

    virtual void on_tick(double dt) = 0;
    virtual void on_settings_update() = 0;

    virtual PreviewLayerPolicy get_flush_policy() = 0;

    virtual void apply() = 0;

    virtual void on_press(Vec2f mouse_pos) = 0;
    virtual void on_release(Vec2f mouse_pos) = 0;
    virtual void on_move(Vec2f mouse_old_pos, Vec2f mouse_new_pos) = 0;
};

struct PAppInterface {
    uint32_t std_version;
    void *reserved;

    // enables specified extension
    virtual bool enable(std::string_view name) = 0;

    // returns given function, if it is implemented in the specified (enabled) extension
    virtual void *get_func(std::string_view extension, std::string_view func) = 0;
    
    int feature_level;

    virtual void log(const char *fmt, ...) = 0;
    virtual double get_absolute_time() = 0;

    virtual void release_pixels(RGBA *pixels) = 0;

    virtual RGBA get_color() = 0;
    virtual float get_size() = 0;

    virtual RenderTarget *get_target();
    virtual RGBA *get_target_pixels();
    virtual void get_size(size_t *width, size_t *height);

    virtual void render_circle(Vec2f position, float radius, RGBA color, const RenderMode *render_mode) = 0;
    virtual void render_line(Vec2f start, Vec2f end, RGBA color, const RenderMode *render_mode) = 0;
    virtual void render_triangle(Vec2f p1, Vec2f p2, Vec2f p3, RGBA color, const RenderMode *render_mode) = 0;
    virtual void render_rectangle(Vec2f p1, Vec2f p2, RGBA color, const RenderMode *render_mode) = 0;

    virtual void render_pixels(Vec2f position, const RGBA *data, size_t width, size_t height, const RenderMode *render_mode) = 0;

    // set everything to nullptr here if you don't support shaders
    virtual void shader_apply(const RenderMode *render_mode);

    virtual void *shader_compile(const char *code, ShaderType type);
    virtual void  shader_release(void *);

    virtual void shader_set_uniform_int      (void *shader, const char *name, int  val);
    virtual void shader_set_uniform_int_arr  (void *shader, const char *name, int *val, size_t cnt);

    virtual void shader_set_uniform_float    (void *shader, const char *name, float  val);
    virtual void shader_set_uniform_float_arr(void *shader, const char *name, float *val, size_t cnt);
};

}

#endif
