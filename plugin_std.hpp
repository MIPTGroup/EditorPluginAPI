#ifndef PLUGIN_STD_HPP
#define PLUGIN_STD_HPP


#include <string>

#include "types_std.h"
#include "widget_std.h"
#include "widgets/collection.h"


// this function is only defined in plugin. call it to get PluginInterface to interact with plugin
// make sure you wrap it into extern C section to avoid mangling
// const PluginInterface *get_plugin_interface();

#if __cplusplus >= 201703L

constexpr char PGET_INTERFACE_FUNC[] = "get_plugin_interface";
constexpr uint32_t PSTD_VERSION = 0x00010000;

constexpr char PEXT_STD[] = "std";

#else

#define PGET_INTERFACE_FUNC "get_plugin_interface"
#define PSTD_VERSION 0x00010000

#define PEXT_STD  "std"

#endif


namespace P {


struct RenderTarget {
    size_t width;
    size_t height;

    virtual RGBA get_pixel(size_t x, size_t y) = 0;
    virtual void set_pixel(size_t x, size_t y, RGBA color) = 0;

    virtual RGBA *get_pixels() = 0;

    virtual void clear(RGBA color = 0) = 0; // fills the target with `color`

// render
    virtual void render_circle(Vec2f position, float radius, RGBA color, const RenderMode *render_mode) = 0;
    virtual void render_line(Vec2f start, Vec2f end, RGBA color, const RenderMode *render_mode) = 0;
    virtual void render_triangle(Vec2f p1, Vec2f p2, Vec2f p3, RGBA color, const RenderMode *render_mode) = 0;
    virtual void render_rectangle(Vec2f p1, Vec2f p2, RGBA color, const RenderMode *render_mode) = 0;
    
    virtual void render_texture(Vec2f position, const RenderTarget *texture, const RenderMode *render_mode) = 0;
    virtual void render_pixels(Vec2f position, const RGBA *data, size_t width, size_t height, const RenderMode *render_mode) = 0;

    virtual void apply(const Shader *shader) = 0;
};


struct PluginInterface;

struct PluginInfo {
    uint32_t std_version;
    void *reserved;

    const PluginInterface *interface;

    const char *name;
    const char *version;
    const char *author;
    const char *description;
    RenderTarget *icon;

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

    virtual void on_press  (Vec2f position) = 0;
    virtual void on_release(Vec2f position) = 0;
    virtual void on_move   (Vec2f from, Vec2f to) = 0;
};

struct WidgetFactory {
    virtual Button      *button       (const WBody &body, Widget *parent = nullptr) = 0;
    virtual Slider      *slider       (const WBody &body, Widget *parent = nullptr) = 0;
    virtual TextField   *text_field   (const WBody &body, Widget *parent = nullptr) = 0;
    virtual Window      *window       (const WBody &body, Widget *parent = nullptr) = 0;
    virtual ColorPicker *color_picker (const WBody &body, Widget *parent = nullptr) = 0;
    virtual Label       *label        (const WBody &body, Widget *parent = nullptr) = 0;
    virtual Widget      *abstract     (const WBody &body, Widget *parent = nullptr) = 0;
};

struct ShaderFactory {
    virtual Shader *compile(const char *code, ShaderType type) = 0;
    virtual void    release(Shader *) = 0;
};

struct RenderTargetFactory {
    virtual RenderTarget *spawn(size_t width, size_t height, RGBA color = {0, 0, 0, 255}) const = 0; // color -> fill with it
    virtual RenderTarget *from_pixels(size_t width, size_t height, RGBA *data) const = 0;
    virtual RenderTarget *from_file(const char *filename) const = 0;
    virtual void release(RenderTarget *target) const = 0;
};

struct AppInterface {
    uint32_t std_version;
    void *reserved;

    int feature_level;

    struct {
        WidgetFactory       *widget;
        ShaderFactory       *shader;
        RenderTargetFactory *target;
    } factory;

// extension
    // enables specified extension
    virtual bool enable(std::string_view name) const = 0;

    // returns given function, if it is implemented in the specified (enabled) extension
    virtual void *get_func(std::string_view extension, std::string_view func) const = 0;

// general
    virtual void log(const char *fmt, ...) const = 0;
    virtual double get_absolute_time() const     = 0;

    virtual RGBA get_color() = 0;
    virtual float get_size() = 0;

// target
    virtual RenderTarget *get_target() const  = 0;
    virtual RenderTarget *get_preview() const = 0;
    virtual void flush_preview() const = 0;
};

}

#endif
