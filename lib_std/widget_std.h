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


class Widget {
public:
    Widget(const WBody &body, Widget *parent = nullptr) : body(body), parent(parent), texture(nullptr) {}

    void set_position(const Vec2f &position_) { body.position = position_; }
    void set_size(const Vec2f &size_) { body.size = size_; }

    WBody &get_body() { return body; };
    void set_body(const WBody &body_) { body = body_; }

    Widget *get_parent() const { return parent; }
    void set_parent(Widget *parent_) { parent = parent; }

    RenderTarget *get_texture() { return texture; }
    void set_texture(RenderTarget *texture_) { texture = texture_; }

    virtual bool is_active() = 0;
    virtual bool is_inside(Vec2f pos) = 0;

    virtual bool add_child(Widget *child) = 0;
    virtual bool delete_child(Widget *child) = 0;

    virtual bool delete_from_parent() = 0;
    
    virtual void on_render          (Widget *self, const Event::Render          &event) = 0;
    virtual void on_tick            (Widget *self, const Event::Tick            &event) = 0;
    virtual void on_mouse_press     (Widget *self, const Event::MousePress      &event) = 0;
    virtual void on_mouse_release   (Widget *self, const Event::MouseRelease    &event) = 0;
    virtual void on_mouse_move      (Widget *self, const Event::MouseMove       &event) = 0;
    virtual void on_key_down        (Widget *self, const Event::KeyDown         &event) = 0;
    virtual void on_key_up          (Widget *self, const Event::KeyUp           &event) = 0;
    virtual void on_text_enter      (Widget *self, const Event::TextEnter       &event) = 0;
    virtual void on_scroll          (Widget *self, const Event::Scroll          &event) = 0;

    virtual void add_label(const char *text, size_t font_size, const Vec2f *pos = nullptr);

    // virtual void on_hover_begin     (Widget *self, const Event:: &event);
    // virtual void on_hover_end       (Widget *self, const Event:: &event);

protected:
    Widget *parent;
    WBody body;
    RenderTarget *texture;
};

}

#endif // WIDGET_STD_H
