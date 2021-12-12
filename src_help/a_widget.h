#include "redactor/plugin_std/std.hpp"


class AbstractWidget : public P::Widget {
public:
    AbstractWidget(const P::WBody &body, P::Widget *parent = nullptr) : Widget(body, parent) {
        if (parent) {
            parent->add_child(this);
        }
    }
    virtual ~AbstractWidget() {}

    virtual bool is_active() override { return true; };
    virtual bool is_inside(P::Vec2f pos) override {
        return (pos.x >= 0) && (pos.y >= 0) && (pos.x < body.size.x) && (pos.y < body.size.y);
    }

    virtual bool add_child(Widget */*child*/) override { return false; }
    virtual bool delete_child(Widget */*child*/) override { return false; }

    virtual bool delete_from_parent() override { return false; }

    virtual void set_to_delete() override {};
    
    virtual void on_render          (const P::Event::Render          &) override {}
    virtual void on_tick            (const P::Event::Tick            &) override {}
    virtual void on_mouse_press     (const P::Event::MousePress      &) override {}
    virtual void on_mouse_release   (const P::Event::MouseRelease    &) override {}
    virtual void on_mouse_move      (const P::Event::MouseMove       &) override {}
    virtual void on_key_down        (const P::Event::KeyDown         &) override {}
    virtual void on_key_up          (const P::Event::KeyUp           &) override {}
    virtual void on_text_enter      (const P::Event::TextEnter       &) override {}
    virtual void on_scroll          (const P::Event::Scroll          &) override {}
    virtual void on_hide            (const P::Event::Hide            &) override {}
    virtual void on_show            (const P::Event::Show            &) override {}

    virtual void hide() override {}
    virtual void show() override {}

    virtual void set_caption(const char */*text*/, size_t /*font_size*/, const P::Vec2f */*pos*/ = nullptr) override {}
    virtual void set_base_color(P::RGBA /*color*/) override {};
};
