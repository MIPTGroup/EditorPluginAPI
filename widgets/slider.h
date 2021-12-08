#include "../widget_std.h"


namespace P {

class Slider : public Widget {
public:
    Slider(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void(Vec2f)>;

    void set_handler(HandlerType &handler_) { handler = handler_; }
    HandlerType &get_handler() { return handler; }

    virtual Vec2f get_fraction() = 0;
    virtual void set_fraction(Vec2f frac) = 0;

protected:
    HandlerType handler;
};

}
