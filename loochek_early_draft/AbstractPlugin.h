#ifndef ABSTRACT_PLUGIN_H
#define ABSTRACT_PLUGIN_H

typedef void* RenderTarget;

struct Vec2f
{
    float x;
    float y;
};

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

typedef void  (*RendererDrawCircleFunc)   (RenderTarget target, Vec2f position, float radius, Color color);
typedef void  (*RendererDrawRectFunc)     (RenderTarget target, Vec2f position, Vec2f size, Color color);
typedef void* (*RendererGetPixelArrayFunc)(RenderTarget target, int *width, int *height);
typedef void  (*RendererSetPixelArrayFunc)(RenderTarget target, void* pixelArray);

struct Renderer
{
    RendererDrawCircleFunc    drawCircle;
    RendererDrawRectFunc      drawRect;
    RendererGetPixelArrayFunc getPixelArray;
    RendererSetPixelArrayFunc setPixelArray;
};

enum PluginType
{
    ToolPlugin,
    EffectPlugin
};

struct PluginInfo
{
    const char *name;
    const char *version;
    const char *author;
    PluginType  pluginType;
};

typedef void (*GetInfoFunc)(struct PluginInfo *structPtr);

typedef struct Renderer *(*PassRendererFunc)();

typedef void (*OnTargetMouseClickFunc)  (RenderTarget target, Vec2f mousePos);
typedef void (*OnTargetMouseMoveFunc)   (RenderTarget target, Vec2f mousePos);
typedef void (*OnTargetMouseReleaseFunc)(RenderTarget target, Vec2f mousePos);

typedef void (*TargetApplyFunc)(RenderTarget target);

typedef void (*OnColorChangeFunc)(Color color);
typedef void (*OnSizeChangeFunc) (float size);

struct PluginInterface
{
    GetInfoFunc getInfo;
    PassRendererFunc passRenderer;

    OnTargetMouseClickFunc   onTargetMouseClick;
    OnTargetMouseMoveFunc    onTargetMouseMove;
    OnTargetMouseReleaseFunc onTargetMouseRelease;

    TargetApplyFunc targetApply;

    OnColorChangeFunc onColorChange;
    OnSizeChangeFunc  onSizeChange;
};

struct PluginInterface *getPluginInterface();

#endif