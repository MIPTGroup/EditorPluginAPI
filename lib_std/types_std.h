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

template <typename T>
union Vec2T {
    struct {
        T x;
        T y;
    };
    T data[2];

    Vec2T()         : x(0),   y(0)   {}
    Vec2T(T val)    : x(val), y(val) {}
    Vec2T(T x, T y) : x(x),   y(y)   {}
};

using Vec2f = Vec2T<float>;
using Vec2d = Vec2T<double>;
using Vec2i = Vec2T<int>;
using Vec2s = Vec2T<size_t>;


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

struct Shader {
    virtual void shader_set_uniform_int      (void *shader, const char *name, int  val) = 0;
    virtual void shader_set_uniform_int_arr  (void *shader, const char *name, int *val, size_t cnt) = 0;

    virtual void shader_set_uniform_float    (void *shader, const char *name, float  val) = 0;
    virtual void shader_set_uniform_float_arr(void *shader, const char *name, float *val, size_t cnt) = 0;
};

struct RenderMode {
    BlendMode blend;
    Shader *shader;
};

}

#endif // TYPES_STD_H
