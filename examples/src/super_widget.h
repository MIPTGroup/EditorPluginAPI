#include "abstract_widget.h"

class RadioButton : public AbstractWidget {
public:
    RadioButton(const P::WBody &body, P::Widget *parent = nullptr) : AbstractWidget(body, parent) {}
    virtual ~RadioButton() {}

    virtual bool get_on() const = 0;
    virtual void set_on(bool flag) = 0;

    using HandlerType = std::function<void()>;

    virtual void set_handler(const HandlerType &handler_) = 0;
    virtual HandlerType &get_handler() = 0;
};

struct SuperWidgetFabric {
    virtual ~SuperWidgetFabric() {}
    virtual RadioButton *radio_button(const P::WBody &body, P::Widget *parent = nullptr, P::RGBA off = {200, 100, 100}, P::RGBA on = {100, 200, 100}) = 0;
};
