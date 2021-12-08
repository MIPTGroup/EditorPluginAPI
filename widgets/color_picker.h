#include "../widget_std.h"


namespace P {

class ColorPicker : public Widget {
public:
    ColorPicker(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void(RGBA)>;

    void set_handler(HandlerType &handler_) { handler = handler_; }
    HandlerType &get_handler() { return handler; }

    virtual RGBA get_color() = 0;
    virtual void set_color(RGBA color) = 0;

protected:
    HandlerType handler;
};

}
