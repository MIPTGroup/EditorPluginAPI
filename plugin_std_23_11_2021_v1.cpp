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
    PRGBA(int x);
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


enum PluginType {
    PPT_EFFECT,
    PPT_TOOL,
};

enum PMouseButton {
    PMB_LEFT,
    PMB_RIGHT,
    PMB_MIDDLE,
};

enum PreviewLayerPolicy {
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


struct PluginInterface;

struct PluginInfo {
    uint32_t std_version;
    void *reserved;

    PluginInterface *interface;

    const char *name;
    const char *version;
    const char *author;
    const char *description;

    PluginType type;
};


struct PluginInterface {
    uint32_t std_version;
    void *reserved;

    void *(*get_extension_func)(const char *name); // runs given func with given args, interprenting them freely

    struct {
        const PluginInfo *(*get_info)();
        PluginStatus (*init)(const AppInterface*);
        PluginStatus (*deinit)();
        void (*dump)();

        void (*on_tick)(double dt);

        PreviewLayerPolicy (*get_flush_policy)();
    } general;

    union {
        struct {
            void (*apply)();
        } effect;

        struct {
            void (*on_press)(PVec2f, PMouseButton);
            void (*on_release)(PVec2f, PMouseButton);
            void (*on_move)(PVec2f, PVec2f, PMouseButton);
        } tool;
    }
};


struct AppInterface {
    uint32_t std_version;
    void *reserved;

    bool (*enable_extension)(const char *name); // enables specified extension
    void *(*get_extension_func)(const char *name); // returns given function, if it is implemented in some enabled extension

    struct {
        void (*log)(const char *, ...);
        double (*get_absolute_time)();

        void (*release_pixels)(PRGBA *);
    } general;

    struct {
	    PRGBA *(*get_pixels)();
	    void (*get_size)(size_t *width, size_t *height);
	} target;

    struct {
        void (*circle)(PVec2f, float, PRGBA, *PRenderMode);
        void (*line)(PVec2f, PVec2f, PRGBA, *PRenderMode);
        void (*triangle)(PVec2f, PVec2f, PVec2f, float, PRGBA, *PRenderMode);
        void (*rectangle)(PVec2f, PVec2f, float, PRGBA, *PRenderMode);

        void (*pixels)(PVec2f position, PRGBA *data, size_t width, size_t height, *PRenderMode);
    } render;

    // set everything to nullptr here if you don't support shaders
    struct {
        void (*apply)(void *shader, *PRenderMode);
        void *(*compile)(const char *code);
        void (*release)(void *);

        void (*set_uniform)(const char *name, int  val);
        void (*set_uniform)(const char *name, int *val, size_t cnt);

        void (*set_uniform)(const char *name, float  val);
        void (*set_uniform)(const char *name, float *val, size_t cnt);

        void (*set_uniform)(const char *name, void *texture);
    } shader;
};
