#ifndef WIDGET_STD_H
#define WIDGET_STD_H

#include <functional>

#include "types_std.h"
#include "events_std.h"


namespace P {

struct WBody {
    Vec2f position;
    Vec2f size;
};

class RenderTarget;

class Widget {
public:
    Widget(): body({{0, 0}, {0, 0}}), parent(nullptr), texture(nullptr) {}
    Widget(const WBody &body, Widget *parent = nullptr) : body(body), parent(parent), texture(nullptr) {}
    virtual ~Widget() {}

    void set_position(const Vec2f &position_) { body.position = position_; }
    void set_size(const Vec2f &size_) { body.size = size_; }

    WBody &get_body() { return body; }
    void set_body(const WBody &body_) { body = body_; }

    Widget *get_parent() const { return parent; }
    void set_parent(Widget *parent_) { parent = parent_; }

    RenderTarget *get_texture() { return texture; }
    void set_texture(RenderTarget *texture_) { texture = texture_; }

    virtual bool is_active() = 0;
    virtual bool is_inside(Vec2f pos) = 0;

    virtual bool add_child(Widget *child) = 0;

    virtual void set_to_delete(bool to_delete_) = 0; // set to true -> app will try to delete it as soon as possible from its side
                                                     // after once setting to true you can not use this widget anymore, it can 
                                                     // already be deleted
    virtual bool delete_child(Widget *child) = 0;
    virtual bool delete_from_parent() = 0;
    
    virtual void on_render          (const Event::Render          &event) = 0;
    virtual void on_tick            (const Event::Tick            &event) = 0;
    virtual void on_mouse_press     (const Event::MousePress      &event) = 0;
    virtual void on_mouse_release   (const Event::MouseRelease    &event) = 0;
    virtual void on_mouse_move      (const Event::MouseMove       &event) = 0;
    virtual void on_key_down        (const Event::KeyDown         &event) = 0;
    virtual void on_key_up          (const Event::KeyUp           &event) = 0;
    virtual void on_text_enter      (const Event::TextEnter       &event) = 0;
    virtual void on_scroll          (const Event::Scroll          &event) = 0;
    virtual void on_hide            (const Event::Hide            &event) = 0;
    virtual void on_show            (const Event::Show            &event) = 0;

    virtual void hide() = 0;
    virtual void show() = 0;

    virtual void set_caption(const char *text, size_t font_size, const Vec2f *pos = nullptr) = 0;
    virtual void set_color(P::RGBA color) = 0;

protected:
    WBody body;
    Widget *parent;
    RenderTarget *texture;
};

}

#endif // WIDGET_STD_H
