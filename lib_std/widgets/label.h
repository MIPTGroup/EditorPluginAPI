#include "../widget_std.h"
#include <string>


namespace P {

class Label : virtual public Widget {
public:
    Label(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    virtual std::string_view get_text() const = 0;
    virtual void set_text(std::string_view text) = 0;
};

}
