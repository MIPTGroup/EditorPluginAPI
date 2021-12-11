#include "../widget_std.h"


namespace P {

class Button : virtual public Widget {
public:
    Button(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void()>;

    virtual void set_handler(const HandlerType &handler_) = 0;
    virtual HandlerType &get_handler() = 0;
};

}
