#ifndef PLUGIN_STD_HPP
#define PLUGIN_STD_HPP


#include <string_view>

#include "lib_std/types_std.h"
#include "lib_std/widgets/collection.h"


// this function is only defined in plugin. call it to get PluginInterface to interact with plugin
// make sure you wrap it into extern C section to avoid mangling
// const PluginInterface *get_plugin_interface();

constexpr char PGET_INTERFACE_FUNC[] = "get_plugin_interface";
constexpr uint32_t PSTD_VERSION = 0x00010000;

constexpr char PEXT_STD[] = "std";


namespace P {


class RenderTarget {
public:
    virtual RenderTarget *get_copy() const = 0;

    virtual Vec2s get_size() const = 0;

    virtual RGBA get_pixel(size_t x, size_t y) const = 0;
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

    virtual void apply_shader(const Shader *shader) = 0;
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
    const RenderTarget *icon;

    const PluginType type;
};

struct AppInterface;
struct PluginInterface {
    uint32_t std_version;
    void *reserved;

    // enables specified extension
    virtual bool enable(const char *name) const = 0;

    // returns given function, if it is implemented in the specified (enabled) extension
    virtual void *get_func(const char *extension, const char *func) const = 0;

    // returns given interface, if it is implemented in the specified (enabled) extension
    virtual void *get_interface(const char *extension, const char *name) const = 0;

    virtual const  PluginInfo *get_info()    const = 0;
    virtual Status init(const AppInterface*) const = 0;
    virtual Status deinit()                  const = 0;
    virtual void   dump()                    const = 0;

    virtual void on_tick(double dt)   const = 0;

    virtual PreviewLayerPolicy get_flush_policy() const = 0;

    virtual void effect_apply() const = 0;

    virtual void tool_on_press  (Vec2f position)       const = 0;
    virtual void tool_on_release(Vec2f position)       const = 0;
    virtual void tool_on_move   (Vec2f from, Vec2f to) const = 0;
};

struct WidgetFactory {
    virtual Button      *button       (const WBody &body, Widget *parent = nullptr) const = 0;
    virtual Slider      *slider       (const WBody &body, Widget *parent = nullptr) const = 0;
    virtual TextField   *text_field   (const WBody &body, Widget *parent = nullptr) const = 0;
    virtual Window      *window       (const WBody &body, Widget *parent = nullptr) const = 0;
    virtual ColorPicker *color_picker (const WBody &body, Widget *parent = nullptr) const = 0;
    virtual Label       *label        (const WBody &body, Widget *parent = nullptr) const = 0;
    virtual Widget      *abstract     (const WBody &body, Widget *parent = nullptr) const = 0;
};

struct ShaderFactory {
    virtual Shader *compile(const char *code, ShaderType type) const = 0;
    virtual void    release(Shader *)                          const = 0;
};

struct RenderTargetFactory {
    virtual RenderTarget *spawn(Vec2s size, RGBA color = {0, 0, 0, 255}) const = 0; // color -> fill with it
    virtual RenderTarget *from_pixels(Vec2s size, const RGBA *data) const = 0;
    virtual RenderTarget *from_file(const char *path) const = 0;
    virtual void release(RenderTarget *target) const = 0;
    virtual void release(RGBA *data) const = 0;
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
    
    // returns given interface, if it is implemented in the specified (enabled) extension
    virtual void *get_interface(const char *extension, const char *name) const = 0;

// general
    virtual void log(const char *fmt, ...) const = 0;
    virtual double get_absolute_time()     const = 0;

    virtual RGBA get_color() const = 0;
    virtual float get_size() const = 0;

// target
    virtual RenderTarget *get_target()  const = 0; // returns actual active  layer, drawing in it changes app's layer
    virtual RenderTarget *get_preview() const = 0; // returns actual preview layer, drawing in it changes app's layer
    virtual void flush_preview()        const = 0;
};

}

#endif
