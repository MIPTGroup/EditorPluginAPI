#include <cstdint>
#include <cmath>


// this function is only defined in plugin. call it to get PPluginInterface to interact with plugin
// struct PPluginInterface *get_plugin_interface();

#if __cplusplus >= 201703L

constexpr char PGET_INTERFACE_FUNC[] = "get_plugin_interface";
constexpr uint32_t PSTD_VERSION = 1;

#else

#define PGET_INTERFACE_FUNC "get_plugin_interface"
#define PSTD_VERSION 1

#endif


union PVec2f {
    struct {
        float x;
        float y;
    };
    float data[2];

    PVec2f()                 : x(0),   y(0)   {}
    PVec2f(float val)        : x(val), y(val) {}
    PVec2f(float x, float y) : x(x),   y(y)   {}
};

union PRGBA {
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    unsigned char rgba[4];
    uint32_t ui32;

    PRGBA()           : ui32(0) {}
    PRGBA(uint32_t x) : ui32(x) {}

    PRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
};


enum PPluginStatus {
    PPS_OK,
    PPS_ERR,
};

enum PFeatureLevel {
    PFL_SHADER_SUPPORT = 1,
};

enum PPluginType {
    PPT_EFFECT,
    PPT_TOOL,
};

enum PPreviewLayerPolicy {
    PPLP_BLEND,
    PPLP_COPY,
};

enum PBlendMode {
    PPBM_COPY,
    PPBM_ALPHA_BLEND,
};

enum PDrawPolicy {
    PPDP_PREVIEW,
    PPDP_ACTIVE,
};

struct PRenderMode {
    PBlendMode blend;
    PDrawPolicy draw_policy;
};


struct PPluginInterface;

struct PPluginInfo {
    uint32_t std_version;
    void *reserved;

    PPluginInterface *interface;

    const char *name;
    const char *version;
    const char *author;
    const char *description;

    PPluginType type;
};


struct PPluginInterface {
    uint32_t std_version;
    void *reserved;

    struct {
        bool  (*enable_extension)(const char *name);   // enables specified extension
        void *(*get_extension_func)(const char *name); // returns given function, if it is implemented in some enabled extension
    } extensions;

    struct {
        const PPluginInfo *(*get_info)();
        PPluginStatus (*init)(const PAppInterface*);
        PPluginStatus (*deinit)();
        void (*dump)();

        void (*on_tick)(double dt);

        PPreviewLayerPolicy (*get_flush_policy)();
    } general;

    union {
        struct {
            void (*apply)();
        } effect;

        struct {
            void (*on_press)(PVec2f mouse_pos);
            void (*on_release)(PVec2f mouse_pos);
            void (*on_move)(PVec2f mouse_old_pos, PVec2f mouse_new_pos);
        } tool;
    };
};


struct PAppInterface {
    uint32_t std_version;
    void *reserved;

    struct {
        bool  (*enable_extension)(const char *name);   // enables specified extension
        void *(*get_extension_func)(const char *name); // returns given function, if it is implemented in some enabled extension
    } extensions;

    struct {
        PFeatureLevel feature_level;

        void (*log)(const char *fmt, ...);
        double (*get_absolute_time)();

        void (*release_pixels)(PRGBA *pixels);

        PRGBA (*get_color)();
        float (*get_size)();
    } general;

    struct {
	    PRGBA *(*get_pixels)();
	    void (*get_size)(size_t *width, size_t *height);
	} target;

    struct {
        void (*circle)(PVec2f position, float radius, PRGBA color, const PRenderMode *render_mode);
        void (*line)(PVec2f start, PVec2f end, PRGBA color, const PRenderMode *render_mode);
        void (*triangle)(PVec2f p1, PVec2f p2, PVec2f p3, PRGBA color, const PRenderMode *render_mode);
        void (*rectangle)(PVec2f p1, PVec2f p2, PRGBA color, const PRenderMode *render_mode);

        void (*pixels)(PVec2f position, const PRGBA *data, size_t width, size_t height, const PRenderMode *render_mode);
    } render;

    // set everything to nullptr here if you don't support shaders
    struct {
        void (*apply)(void *shader, const PRenderMode*render_mode);

        void *(*compile)(const char *code);
        void  (*release)(void *);

        void (*set_uniform_int)    (const char *name, int  val);
        void (*set_uniform_int_arr)(const char *name, int *val, size_t cnt);

        void (*set_uniform_float)    (const char *name, float  val);
        void (*set_uniform_float_arr)(const char *name, float *val, size_t cnt);

        void (*set_uniform_tex)(const char *name, void *texture);
    } shader;
};
