#include "redactor/plugin_std.h"
// #include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const uint32_t PSTDVERSION = 0;

const PPluginType PTYPE = PPT_EFFECT;

const char *PNAME    = "Negator";
const char *PVERSION = "1.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Negotiates with you colors";

// ============================================================================ Flush policy

const PPreviewLayerPolicy FLUSH_POLICY = PPLP_BLEND;

// ============================================================================ Resources

void *r_shader_neg = nullptr;

// ============================================================================


static PPluginStatus init(const PAppInterface* appInterface);
static PPluginStatus deinit();

static void dump();
static void on_tick(double dt);
static void on_update();

static const PPluginInfo  *get_info();
static PPreviewLayerPolicy get_flush_policy();

static void on_mouse_down(PVec2f pos);
static void on_mouse_move(PVec2f from, PVec2f to);
static void on_mouse_up  (PVec2f pos);
static void apply();

static bool  enable_extension  (const char *name);
static void *get_extension_func(const char *name);


const PPluginInterface PINTERFACE =
{
    0, // std_version
    0, // reserved
    
    {
        enable_extension,
        get_extension_func,
    },

    // general
    {
        get_info,
        init,
        deinit,
        dump,
        on_tick,
        on_update,
        get_flush_policy,
    },

    // effect
    {
        apply,
    },

    // tool
    {
        on_mouse_down,
        on_mouse_up  ,
        on_mouse_move,
    },
};

const PPluginInfo PINFO =
{
    PSTDVERSION, // std_version
    nullptr,     // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,
    
    PTYPE
};

const PAppInterface *APPI = nullptr;


extern "C" const PPluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}

static PPluginStatus init(const PAppInterface *app_interface) {
    srand(time(NULL));

    APPI = app_interface;

    if (APPI->general.feature_level & PFL_SHADER_SUPPORT) {
        r_shader_neg = APPI->shader.compile(
           "uniform sampler2D texture;                                                  \
            void main() {                                                               \
                vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);                     \
                vec4 color = vec4(1.0 - pixel.r, 1.0 - pixel.g, 1.0 - pixel.b, pixel.w);\
                gl_FragColor = color;                                                   \
            }                                                                           \
           ", PST_FRAGMENT);
    }

    APPI->general.log("[plugin](%s) inited", PINFO.name);
    return PPS_OK;
}

static PPluginStatus deinit() {
    if (r_shader_neg) {
        APPI->shader.release(r_shader_neg);
    }

    APPI->general.log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return PPS_OK;
}

static void dump() {
    APPI->general.log("[plugin](%s) is active", PINFO.name);
}

static const PPluginInfo *get_info() {
    return &PINFO;
}

static void on_tick(double /*dt*/) {
}

static void on_update() {
}

static PPreviewLayerPolicy get_flush_policy() {
    return FLUSH_POLICY;
}

static void on_mouse_down(PVec2f /*pos*/) {
}

static void on_mouse_move(PVec2f /*from*/, PVec2f /*to*/) {
}

static void on_mouse_up(PVec2f /*pos*/) {}

static void apply() {
    if (r_shader_neg) {
        PRenderMode render_mode = { PPBM_COPY, PPDP_ACTIVE, r_shader_neg };
        APPI->shader.apply(&render_mode);
        return;
    }

    auto pixels = APPI->target.get_pixels();
    size_t w = 0;
    size_t h = 0;
    APPI->target.get_size(&w, &h);
    size_t sz = w * h;
    for (size_t i = 0; i < sz; ++i) {
        PRGBA c = pixels[i];
        pixels[i] = {
            (unsigned char) (255 - c.r),
            (unsigned char) (255 - c.g),
            (unsigned char) (255 - c.b),
            c.a,
        };
    }

    PRenderMode rmode = {PPBM_COPY, PPDP_ACTIVE, nullptr};
    APPI->render.pixels({0, 0}, pixels, w, h, &rmode);
    APPI->general.release_pixels(pixels);
}

static bool enable_extension(const char * /*name*/) {
    return false;
}

static void *get_extension_func(const char * /*name*/) {
    return nullptr;
}
