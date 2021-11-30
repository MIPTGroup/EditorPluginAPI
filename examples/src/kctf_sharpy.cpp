#include "redactor/plugin_std.h"
#include "utils.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const uint32_t PSTDVERSION = 0;

const char *PNAME    = "Sharpy";
const char *PVERSION = "1.0";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "Spawns random triangles in a circle, blends them on active layer";

// ============================================================================ Flush policy

const PPreviewLayerPolicy FLUSH_POLICY = PPLP_BLEND;

// ============================================================================ Resources

void *r_max_size_setting = nullptr;
void *r_size_setting = nullptr;
void *r_color_setting = nullptr;

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

static void draw(PVec2f pos);


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
    
    PPT_TOOL
};

const PAppInterface *APPI = nullptr;


extern "C" const PPluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}

static PPluginStatus init(const PAppInterface *app_interface) {
    srand(time(NULL));

    APPI = app_interface;

    if (APPI->general.feature_level & PFL_SETTINGS_SUPPORT) {
        APPI->settings.create_surface(&PINTERFACE, 200, 200);
        r_max_size_setting = APPI->settings.add(&PINTERFACE, PST_TEXT_LINE, "Max");
        r_size_setting = APPI->settings.add(&PINTERFACE, PST_SLIDER_1D, "Size");
        r_color_setting = APPI->settings.add(&PINTERFACE, PST_COLOR_PICKER, "Color");

        APPI->general.log("[plugin](%s) is happy for your settings support!", PINFO.name);
    } else {
        APPI->general.log("[plugin](%s) is NOT happy for you not supporting settings!", PINFO.name);
    }

    APPI->general.log("[plugin](%s) inited", PINFO.name);
    return PPS_OK;
}

static PPluginStatus deinit() {
    if (APPI->general.feature_level & PFL_SETTINGS_SUPPORT) {
        APPI->settings.destroy_surface(&PINTERFACE);
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

static void on_mouse_down(PVec2f pos) {
    draw(pos);
}

static void on_mouse_move(PVec2f /*from*/, PVec2f to) {
    draw(to);
}

static void on_mouse_up(PVec2f /*pos*/) {}

static void apply() {}

static bool enable_extension(const char * /*name*/) {
    return false;
}

static void *get_extension_func(const char * /*name*/) {
    return nullptr;
}

inline unsigned long long read_next_long_long(const char **buffer) {
    const char *c = *buffer;
    while (c && (*c == ' ' || *c == '\n')) ++c;
 
    unsigned long long l = 0;
    while (*c >= '0' && *c <= '9') {
        l = l * 10 + *c - '0';
        ++c;
    }
 
    *buffer = c;
    return l;
}

static void draw(PVec2f pos) {
    float size = APPI->general.get_size();
    PRGBA color = APPI->general.get_color();
    float max_size = 50;

    if (r_max_size_setting) {
        PTextFieldSetting text;
        APPI->settings.get(&PINTERFACE, r_max_size_setting, &text);
        const char *buffer = text.text;
        unsigned long long wanted_size = read_next_long_long(&buffer);
        if (wanted_size) {
            max_size = wanted_size;
        }
    }

    if (r_size_setting) {
        PSlider1dSetting slider;
        APPI->settings.get(&PINTERFACE, r_size_setting, &slider);
        size = 2 + max_size * slider.frac;
    }
    if (r_color_setting) {
        PColorPickerSetting color_picker;
        APPI->settings.get(&PINTERFACE, r_color_setting, &color_picker);
        color = color_picker.color;
    }

    float a1 = rand();
    float a2 = rand();

    PVec2f p0 = pos;
 
    PVec2f p1 = {(float) (pos.x + cos(a1) * size), (float) (pos.y + sin(a2) * size)};
    PVec2f p2 = {(float) (pos.x + cos(a2) * size), (float) (pos.y + sin(a1) * size)};

    PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_ACTIVE, nullptr };
    APPI->render.triangle(p0, p1, p2,
                          color,
                          &render_mode);
}
