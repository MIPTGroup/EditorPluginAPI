#include "redactor/plugin_std.h"

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

    "Neg.Brush",
    "version",
    "KCTF",
    "NOT cute and harmful",
    
    PPT_TOOL
};


const PAppInterface *gAppInterface = nullptr;


extern "C" const PPluginInterface *get_plugin_interface()
{
    return &gPluginInterface;
}

void *shader = nullptr;

static PPluginStatus init(const PAppInterface* appInterface)
{
    gAppInterface = appInterface;

    if (appInterface->general.feature_level & PFL_SHADER_SUPPORT) {

        shader = appInterface->shader.compile("uniform sampler2D texture; uniform float r; uniform float g; uniform float b; void main() \
    {\
        vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);\
        vec4 color = vec4(1.0 - r, 1.0 - g, 1.0 - b, pixel.w);\
        gl_FragColor = color;\
    }\
    ", PST_FRAGMENT);
    }

    if (!shader) {
        appInterface->general.log("I WONT WORK WITHOUT SHADERS (i will)");
    }

    appInterface->general.log("shdr: succesful initialization!");
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
    draw(mouseNewPos);
}

static void onMouseReleased(PVec2f mousePos)
{
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
    float size = gAppInterface->general.get_size();
    PRGBA color = gAppInterface->general.get_color();


    if (!shader) {
        PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_PREVIEW, nullptr };

        gAppInterface->render.circle(mousePos, size, negative(color), &render_mode);

        gAppInterface->general.log("you are bad");
    } else {
        PRenderMode render_mode = { PPBM_ALPHA_BLEND, PPDP_PREVIEW, shader };
        gAppInterface->shader.set_uniform_float(shader, "r", (float) color.r / 255);
        gAppInterface->shader.set_uniform_float(shader, "g", (float) color.g / 255);
        gAppInterface->shader.set_uniform_float(shader, "b", (float) color.b / 255);
        gAppInterface->render.circle(mousePos, size, color, &render_mode);

        // gAppInterface->general.log("you are bad");
    }
}
