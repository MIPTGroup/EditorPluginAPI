#include "abstract_widget.h"

class RadioButton : public AbstractWidget {
    bool on;
    P::RenderTarget *one;
    P::RenderTarget *two;

    const P::AppInterface *APPI;

public:
    RadioButton(const P::AppInterface *app_interface, const P::WBody &body, P::Widget *parent = nullptr, P::RGBA first = {200, 100, 100}, P::RGBA second = {100, 200, 100});
    virtual ~RadioButton();

    virtual void on_mouse_press(const P::Event::MousePress &event) override;

    bool get_on() const;
    void set_on(bool flag);
};

struct SuperWidgetFabric {
    virtual ~SuperWidgetFabric() {}
    virtual RadioButton *radio_button(const P::WBody &body, P::Widget *parent = nullptr, P::RGBA first = {200, 100, 100}, P::RGBA second = {100, 200, 100}) = 0;
};
