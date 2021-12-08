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

struct RenderMode {
    BlendMode blend;
    DrawPolicy draw_policy;
    void *shader;
};

struct RenderTarget {
    size_t width;
    size_t height;
};

}

#endif // TYPES_STD_H