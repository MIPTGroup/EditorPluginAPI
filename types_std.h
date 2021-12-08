#ifndef TYPES_STD_H
#define TYPES_STD_H

#include <cstdint>
#include <cmath>


namespace P {

enum Status {
    PPS_OK,
    PPS_ERR,
};

enum FeatureLevel {
    PFL_SHADER_SUPPORT   = 1 << 0,
    PFL_SETTINGS_SUPPORT = 1 << 1,
};

enum PluginType {
    PPT_EFFECT,
    PPT_TOOL,
};

enum PreviewLayerPolicy {
    PPLP_BLEND,
    PPLP_COPY,
};

enum BlendMode {
    PPBM_COPY,
    PPBM_ALPHA_BLEND,
};

enum DrawPolicy {
    PPDP_PREVIEW,
    PPDP_ACTIVE,
};

enum ShaderType {
    PST_VERTEX,
    PST_FRAGMENT,
    PST_COMPUTE,
};

enum MouseButton {
    none,
    left,
    right,
    middle
};

union Vec2f {
    struct {
        float x;
        float y;
    };
    float data[2];

    Vec2f()                 : x(0),   y(0)   {}
    Vec2f(float val)        : x(val), y(val) {}
    Vec2f(float x, float y) : x(x),   y(y)   {}
};

union RGBA {
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    unsigned char rgba[4];
    uint32_t ui32;

    RGBA()           : ui32(0) {}
    RGBA(uint32_t x) : ui32(x) {}

    RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
};

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
    
    virtual void render_target(Vec2f position, const RGBA *data, size_t width, size_t height, const RenderMode *render_mode) = 0;
    virtual void render_pixels(Vec2f position, const RGBA *data, size_t width, size_t height, const RenderMode *render_mode) = 0;

    virtual void apply(const Shader *shader) = 0;
};

struct Shader {
    virtual void shader_set_uniform_int      (void *shader, const char *name, int  val) = 0;
    virtual void shader_set_uniform_int_arr  (void *shader, const char *name, int *val, size_t cnt) = 0;

    virtual void shader_set_uniform_float    (void *shader, const char *name, float  val) = 0;
    virtual void shader_set_uniform_float_arr(void *shader, const char *name, float *val, size_t cnt) = 0;
};

struct RenderMode {
    BlendMode blend;
    DrawPolicy draw_policy;
    Shader *shader;
};

}

#endif // TYPES_STD_H
