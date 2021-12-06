#include "../plugin_std.hpp"
#include "../../../includes/file/bmpheader.h"
#include <sys/stat.h>
#include "dt.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdio>

// ============================================================================ Info

const uint32_t PSTDVERSION = 0;

const char *PNAME    = "Dream Theater";
const char *PVERSION = "1.0";
const char *PAUTHOR  = "Jules";
const char *PDESCR   = "For those who understad I extend my hand. To the doubtful I demand: take me as I am!";

// ============================================================================ Flush policy

const PPreviewLayerPolicy FLUSH_POLICY = PPLP_BLEND;

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

static void draw(PVec2f pos);


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
    
    PPT_TOOL
};

const PAppInterface *APPI = nullptr;


extern "C" const PPluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}

//
// Defines
//


PRGBA* image;

static PPluginStatus init(const PAppInterface *app_interface) {
    srand(time(NULL));

    APPI = app_interface;

    auto data = header_data;

    image = new PRGBA[dtwidth * dtheight];
    int index = 0;
    for (int y = 0; y != dtheight; y++)
    {
        for (int x = 0; x != dtwidth; x++)
        {
            PRGBA color;
            HEADER_PIXEL(data, ((char*)&color));
            printf("[%3d %3d %3d %3d] ", image[index - 1].r, image[index - 1].g, image[index - 1].b, image[index - 1].a);
            if (color.r != 255) // hardcode
                color.a = 255;

            image[index++] = color;
        }

        printf("\n");
    }

    APPI->general.log("Hello Victoria, so glad to see you, my friend!");
    return PPS_OK;
}

static PPluginStatus deinit() 
{
    delete[] image;
    APPI->general.log("Tell em about my brother, tell em about me -- the Count of Tuscany!");
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
}

static PPreviewLayerPolicy get_flush_policy() {
    return FLUSH_POLICY;
}

static void on_mouse_down(PVec2f pos) {
    draw(pos);
}

static void on_mouse_move(PVec2f /*from*/, PVec2f to) 
{
    
}

static void on_mouse_up(PVec2f /*pos*/) {}

static void apply() {}

static bool enable_extension(const char * /*name*/) 
{
    return false;
}

static void *get_extension_func(const char * /*extension*/, const char * /*func*/) 
{
    return nullptr;
}

static void draw(PVec2f pos) 
{
    PRenderMode mode = 
    {
        .blend = PPBM_ALPHA_BLEND,
        .draw_policy = PPDP_PREVIEW,
    };

    APPI->render.pixels({ pos.x - dtwidth / 2, pos.y - dtheight / 2 }, image, dtwidth, dtheight, &mode);
}
