#include "../widget_std.h"


namespace PUPPY {

class Slider : virtual public Widget {
public:
    enum class Type {
        X, // horizontal. forbids vertical movement, but set_fraction_2d still overwrites thumb position
        Y, // vertical. forbids horizontal movement, but set_fraction_2d still overwrites thumb position
        D2 // 2d - can not be available, check value returned by the factory
    };

    Slider(const WBody &body, Widget *parent = nullptr) : Widget(body, parent) {}

    using HandlerType = std::function<void(Vec2f)>;

    virtual void set_handler(const HandlerType &handler_) { handler = handler_; }
    virtual HandlerType &get_handler() { return handler; }

    virtual Vec2f get_fraction_2d() = 0;
    virtual void set_fraction_2d(Vec2f frac) = 0;

    virtual float get_fraction() = 0;
    virtual void set_fraction(float frac) = 0;

protected:
    HandlerType handler;
};

}
