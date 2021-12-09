#include "../widget_std.h"


namespace P {

class Button : public Widget {
public:
    Button(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void()>;

    void set_handler(HandlerType &handler_) { handler = handler_; }
    HandlerType &get_handler() { return handler; }

protected:
    HandlerType handler;
};

}
