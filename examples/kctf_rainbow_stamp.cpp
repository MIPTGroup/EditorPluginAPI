#include "redactor/plugin_std.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

static PPluginStatus init(const PAppInterface* appInterface);
static PPluginStatus deinit();

static void dump();
static void onUpdate(double elapsedTime);

static const PPluginInfo  *getInfo();
static PPreviewLayerPolicy getFlushPolicy();

static void onMousePressed(PVec2f mousePos);
static void onMouseMove(PVec2f mouseOldPos, PVec2f mouseNewPos);
static void onMouseReleased(PVec2f mousePos);

static bool enableExtension(const char *name);
static void *getExtensionFunc(const char *name);

static void draw(PVec2f mousePos);


const PPluginInterface gPluginInterface =
{
    0, // std_version
    0, // reserved
    
    enableExtension,
    getExtensionFunc,

    // general
    getInfo,
    init,
    deinit,
    dump,
    onUpdate,
    nullptr,
    getFlushPolicy,

    // effect
    nullptr,

    // tool
    onMousePressed,
    onMouseReleased,
    onMouseMove
};

const PPluginInfo gPluginInfo =
{
    0, // std_version
    0, // reserved

    &gPluginInterface,

    "Rainbow Stamp",
    "version",
    "KCTF",
    "Cute and NOT harmful",
    
    PPT_TOOL
};


const PAppInterface *gAppInterface = nullptr;


extern "C" const PPluginInterface *get_plugin_interface()
{
    return &gPluginInterface;
}

static PPluginStatus init(const PAppInterface* appInterface)
{
    srand(time(NULL));

    gAppInterface = appInterface;
    appInterface->general.log("Stamp: succesful initialization!");
    return PPS_OK; 
}

static PPluginStatus deinit()
{
    return PPS_OK;
}

static void dump()
{
}

static const PPluginInfo *getInfo()
{
    return &gPluginInfo;
}

static void onUpdate(double elapsedTime)
{
}

static PPreviewLayerPolicy getFlushPolicy()
{
    return PPLP_BLEND;
}

static void onMousePressed(PVec2f mousePos)
{
    draw(mousePos);
}

static void onMouseMove(PVec2f mouseOldPos, PVec2f mouseNewPos)
{
    // draw(mouseNewPos);
}

static void onMouseReleased(PVec2f mousePos)
{
    gAppInterface->general.log("Congratulations, you have sold your soul to Max Gorishniy!");
}

static bool enableExtension(const char *name)
{
    return false;
}

static void *getExtensionFunc(const char *name)
{
    return nullptr;
}

PRGBA negative(PRGBA col) {
    return {(unsigned char) (255 - col.r), 
            (unsigned char) (255 - col.g),
            (unsigned char) (255 - col.b),
            col.a};
}

PRGBA shift(PRGBA col) {
    return {col.g, col.b, col.r, col.a};
}

static void draw(PVec2f mousePos)
{
    PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_PREVIEW, nullptr };

    float size = gAppInterface->general.get_size() * 2.5;
    PRGBA color = gAppInterface->general.get_color();

    int rnd = rand();

    for (int i = 1; i < 50; ++i) {
        PRGBA col = color;
        if ((i + (rnd % 2)) % 2) {
            col = negative(col);
        }
        for (int j = 0; j < i % 7 + (rnd % 10); ++j) {
            col = shift(col);
        }

        float frac = (float) i / 10;
        frac = exp(frac);

        gAppInterface->render.circle(mousePos, size / frac, col, &render_mode);
    }
}
