#include "../widget_std.h"
#include <string>


namespace P {

class TextField : public Widget {
public:
    TextField(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void(std::string_view)>;

    void set_handler(HandlerType &handler_) { handler = handler_; }
    HandlerType &get_handler() { return handler; }

    virtual std::string_view get_text() const = 0;
    virtual void set_text(std::string_view text) = 0;

protected:
    HandlerType handler;
};

}
