#include "../widget_std.h"


namespace P {

class Window : public Widget {
public:
    Window(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void(Vec2f)>;

    void set_handler(HandlerType &handler_) { handler = handler_; }
    HandlerType &get_handler() { return handler; }

    virtual void hide() = 0;
    virtual void show() = 0;

protected:
    HandlerType handler;
};

}
