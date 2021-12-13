#include "redactor/plugin_std/std.hpp"
#include "utils.h"
#include "plugin_interface.h"

#include "super_widget.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = PUPPY::EXTENSION;

const char *PNAME    = "SuperWidget";
const char *PVERSION = "69";
const char *PAUTHOR  = "KCTF";
const char *PDESCR   = "The most Awasomest WidGetS ever in my humble fabric";

// ============================================================================ Resources

void *r_max_size_setting = nullptr;
void *r_size_setting = nullptr;
void *r_color_setting = nullptr;

SuperWidgetFabric *r_super_fabric = nullptr;

// ============================================================================ General

MyPluginInterface PINTERFACE;

const PUPPY::PluginInfo PINFO =
{
    PUPPY::STD_VERSION, // std_version
    nullptr,     // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,
    nullptr, // icon
    
    PTYPE
};

const PUPPY::AppInterface *APPI = nullptr;


extern "C" const PUPPY::PluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}
 
// ============================================================================ Logic

class MyRadioButton : public RadioButton {
    bool on;
    PUPPY::RenderTarget *one;
    PUPPY::RenderTarget *two;

    const PUPPY::AppInterface *APPI;

    HandlerType handler;

public:
    MyRadioButton(const PUPPY::AppInterface *app_interface, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr, PUPPY::RGBA first = {200, 100, 100}, PUPPY::RGBA second = {100, 200, 100});

    virtual ~MyRadioButton();

    virtual void on_mouse_press(const PUPPY::Event::MousePress &event) override;

    virtual bool get_on() const override;
    virtual void set_on(bool flag) override;

    virtual void set_handler(const HandlerType &handler_) override { handler = handler_; }
    virtual HandlerType &get_handler() override { return handler; }
};

MyRadioButton::MyRadioButton(const PUPPY::AppInterface *app_interface, const PUPPY::WBody &body, PUPPY::Widget *parent, PUPPY::RGBA first, PUPPY::RGBA second) :
RadioButton(body, parent),
// AbstractWidget(body, parent),
on(false),
one(nullptr),
two(nullptr),
APPI(app_interface)
{
    one = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, second);
    two = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, first);

    set_on(on);
}

MyRadioButton::~MyRadioButton() {
    delete one;
    delete two;
}

void MyRadioButton::on_mouse_press(const PUPPY::Event::MousePress &event) {
    if (!is_inside(event.position)) {
        return;
    }

    set_on(!on);
    if (handler) {
        handler();
    }
}

bool MyRadioButton::get_on() const {
    return on;
}

void MyRadioButton::set_on(bool flag) {
    on = flag;
    if (on) {
        texture = one;
    } else {
        texture = two;
    }
}

struct MySuperWidgetFabric : public SuperWidgetFabric {
    virtual RadioButton *radio_button(const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr, PUPPY::RGBA first = {200, 100, 100}, PUPPY::RGBA second = {100, 200, 100}) override {
        return new MyRadioButton(APPI, body, parent, first, second);
    }
};

PUPPY::Status MyPluginInterface::init(const PUPPY::AppInterface *app_interface) const {
    srand(time(NULL));

    APPI = app_interface;

    r_super_fabric = new MySuperWidgetFabric;

    APPI->ext_register_as("KCTFSuperWidget");
    PINTERFACE.exts.set("SuperWidgetFabric", r_super_fabric);

    APPI->log("[plugin](%s) inited", PINFO.name);
    return PUPPY::OK;
}

PUPPY::Status MyPluginInterface::deinit() const {
    delete r_super_fabric;

    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return PUPPY::OK;
}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const PUPPY::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {
}

void MyPluginInterface::tool_on_press(const PUPPY::Vec2f &pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(const PUPPY::Vec2f &/*from*/, const PUPPY::Vec2f &to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(const PUPPY::Vec2f &/*pos*/) const {}

void MyPluginInterface::effect_apply() const {}

inline unsigned long long read_next_long_long(const char **buffer) {
    const char *c = *buffer;
    while (c && (*c == ' ' || *c == '\n')) ++c;
 
    unsigned long long l = 0;
    while (*c >= '0' && *c <= '9') {
        l = l * 10 + *c - '0';
        ++c;
    }
 
    *buffer = c;
    return l;
}

unsigned long long read(const char *text) {
    unsigned long long wanted_size = read_next_long_long(&text);
    return wanted_size;
}

void MyPluginInterface::show_settings() const {}


void func() {
    APPI->log("ALOLOLA");
}

void printstr(const char *str) {
    APPI->log(str);
}

void slfn(PUPPY::Vec2f frac) {
    printf("frac is %f %f\n", frac.x, frac.y);
}

typedef void (*generate_triangle_type)(const PUPPY::Vec2f &pos, float radius, PUPPY::Vec2f &p1, PUPPY::Vec2f &p2, PUPPY::Vec2f &p3);

void MyPluginInterface::draw(const PUPPY::Vec2f &pos) const {
    // auto layout = APPI->factory.widget->window("opts", {pos, {400, 200}});
    // layout->set_name("abobs");

    // auto b1 = APPI->factory.widget->button({0, 0}, "TOUCH ME", layout);
    // b1->set_handler(func);
    
    // PUPPY::Vec2f p = b1->get_body().position;
    // p.x += b1->get_body().size.x;
    // p.y += b1->get_body().size.y;

    // auto b2 = APPI->factory.widget->slider(PUPPY::Slider::Type::X, {p, {100, 40}}, layout);
    // b2->set_base_color({100, 200, 255});
    // b2->set_handler(slfn);

    // p.x += b2->get_body().size.x;
    // p.y += b2->get_body().size.y;

    // auto picker = new Radio({p, {100, 100}}, layout);

    // layout->show();

    float size = APPI->get_size();
    PUPPY::RGBA color = APPI->get_color();  

    APPI->ext_enable("sharpy");
    auto func = (generate_triangle_type) APPI->ext_get_func("sharpy", "generate_triangle");

    PUPPY::Vec2f p0, p1, p2;
    func(pos, size, p0, p1, p2);

    auto target = APPI->get_target();
    
    target->render_triangle(p0, p1, p2, color, PUPPY::COPY);
    
    delete target;
}
