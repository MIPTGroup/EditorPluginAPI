#include "../plugin_std.hpp"
#include <sys/stat.h>

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>

// ============================================================================ Info

const uint32_t PSTDVERSION = 0;

const char *PNAME    = "Chromakey";
const char *PVERSION = "1.0";
const char *PAUTHOR  = "Jules";
const char *PDESCR   = "Lol";

// ============================================================================ Flush policy

const PPreviewLayerPolicy FLUSH_POLICY = PPLP_COPY;

// ============================================================================ Resources

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
static void *get_extension_func(const char *extension, const char *func);

static void draw();


const PPluginInterface PINTERFACE =
{
    PSTDVERSION, // std_version
    nullptr,     // reserved
    
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
    
    PPT_EFFECT
};

const PAppInterface *APPI = nullptr;


extern "C" const PPluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}

//
// Defines
//

static PPluginStatus init(const PAppInterface *app_interface) {
    APPI = app_interface;
    APPI->general.log("Hello from chromakey!");
    return PPS_OK;
}

static PPluginStatus deinit() 
{
    return PPS_OK;
}

static void dump() 
{
    APPI->general.log("");
}

static const PPluginInfo *get_info() 
{
    return &PINFO;
}

static void on_tick(double /*dt*/) 
{

}

static void on_update() 
{
    draw();
}

static PPreviewLayerPolicy get_flush_policy() 
{
    return FLUSH_POLICY;
}

static void on_mouse_down(PVec2f pos) 
{
    
}

static void on_mouse_move(PVec2f /*from*/, PVec2f to) 
{
    
}

static void on_mouse_up(PVec2f /*pos*/) 
{

}

static void apply() 
{
    draw();
}

static bool enable_extension(const char * /*name*/) 
{
    return false;
}

static void *get_extension_func(const char * /*extension*/, const char * /*func*/) 
{
    return nullptr;
}

static void draw() 
{
    PRenderMode mode = 
    {
        .blend = PPBM_COPY,
        .draw_policy = PPDP_PREVIEW,
    };

    size_t width = 0, height = 0;
    APPI->target.get_size(&width, &height);
    auto key = APPI->general.get_color();
    auto image = APPI->target.get_pixels();

    for (int i = 0; i != width * height; i++)
    {
        if (image[i].r == key.r &&
            image[i].g == key.g &&
            image[i].b == key.b)
        {
            image[i] = {0, 0, 0, 0};
        }
    }

    APPI->render.pixels({ 0, 0 }, image, width, height, &mode);
    APPI->general.release_pixels(image);
}
