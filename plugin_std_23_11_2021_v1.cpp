#include <cstdint>
#include <cmath>


// this function is only defined in plugin. call it to get PPluginInterface to interact with plugin
// struct PPluginInterface *get_plugin_interface();

#if __cplusplus >= 201703L

constexpr char GET_PLUGIN_INTERFACE_FUNC[] = "get_plugin_interface";
constexpr uint32_t PSTD_VERSION = 1;

#else

#define GET_PLUGIN_INTERFACE_FUNC "get_plugin_interface"
#define PSTD_VERSION 1

#endif

  
struct PVec2f {
    union {
        struct {
            float x;
            float y;
        };

        float data[2];
    };

    PVec2f(float x, float y) {
        data[0] = x;
        data[1] = y;
    }

    inline float len_squared() const {
        return x * x + y * y;
    }

    inline float len() const {
        return sqrt(len_squared());
    }

    inline void clamp(const PVec2f &vmin, const PVec2f &vmax) {
        for (int coord = 0; coord < 2; ++coord) {
            data[coord] = fmin(fmax(data[coord], vmin.data[coord]), vmax.data[coord]);
        }
    }
};

union PRGBA {
    unsigned char rgba[4];
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    int32_t i;
    uint32_t ui32;

    inline bool is_empty() {
        return ui32 == 0;
    }

    PRGBA();
    PRGBA(int32_t x);
    PRGBA(uint32_t x);
    PRGBA(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 255);

    // invert
    inline PRGBA operator-() const {
        return PRGBA{255 - r, 255 - g, 255 - b, a};
    }

    // Are these supposed to clamp? Not sure, not gonna implement yet
    PRGBA operator+(const PRGBA &other) const;
    PRGBA operator-(const PRGBA &other) const;

    PRGBA operator/(float coef) const;
    PRGBA operator*(float coef) const;

    inline friend PRGBA operator*(float coef, const PRGBA &color) {
        return color * coef;
    }
};

enum PPluginStatus {
    PPS_OK,
    PPS_ERR
};

enum PFeatureLevel {
    PFL_SHADER_SUPPORT = 1
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

    bool  (*enable_extension)(const char *name);   // enables specified extension
    void *(*get_extension_func)(const char *name); // returns given function, if it is implemented in some enabled extension

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

    bool  (*enable_extension)(const char *name);   // enables specified extension
    void *(*get_extension_func)(const char *name); // returns given function, if it is implemented in some enabled extension

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
        void (*release)(void *);

        void (*set_uniform_int)(const char *name, int  val);
        void (*set_uniform_int_arr)(const char *name, int *val, size_t cnt);

        void (*set_uniform_float)(const char *name, float  val);
        void (*set_uniform_float_arr)(const char *name, float *val, size_t cnt);

        void (*set_uniform_tex)(const char *name, void *texture);
    } shader;
};
