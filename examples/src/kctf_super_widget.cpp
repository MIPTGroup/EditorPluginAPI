#include "redactor/plugin_std/std.hpp"
#include "utils.h"
#include "plugin_interface.h"

#include "super_widget.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::EXTENSION;

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

const P::PluginInfo PINFO =
{
    P::STD_VERSION, // std_version
    nullptr,     // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,
    nullptr, // icon
    
    PTYPE
};

const P::AppInterface *APPI = nullptr;


extern "C" const P::PluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}
 
// ============================================================================ Logic

RadioButton::RadioButton(const P::AppInterface *app_interface, const P::WBody &body, P::Widget *parent, P::RGBA first, P::RGBA second) :
AbstractWidget(body, parent),
on(false),
one(nullptr),
two(nullptr),
APPI(app_interface)
{
    one = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, first);
    texture = two = APPI->factory.target->spawn({(size_t) body.size.x, (size_t) body.size.y}, second);
}

RadioButton::~RadioButton() {
    delete one;
    delete two;
}

void RadioButton::on_mouse_press(const P::Event::MousePress &event) {
    if (!is_inside(event.position)) {
        return;
    }

    set_on(!on);
}

bool RadioButton::get_on() const {
    return on;
}

void RadioButton::set_on(bool flag) {
    on = flag;
    if (on) {
        texture = one;
    } else {
        texture = two;
    }
}

struct MySuperWidgetFabric : public SuperWidgetFabric {
    virtual RadioButton *radio_button(const P::WBody &body, P::Widget *parent = nullptr, P::RGBA first = {200, 100, 100}, P::RGBA second = {100, 200, 100}) override {
        return new RadioButton(APPI, body, parent, first, second);
    }
};

P::Status MyPluginInterface::init(const P::AppInterface *app_interface) const {
    srand(time(NULL));

    APPI = app_interface;

    r_super_fabric = new MySuperWidgetFabric;

    APPI->ext_register_as("KCTFSuperWidget");
    PINTERFACE.exts.set("SuperWidgetFabric", r_super_fabric);

    APPI->log("[plugin](%s) inited", PINFO.name);
    return P::OK;
}

P::Status MyPluginInterface::deinit() const {
    delete r_super_fabric;

    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return P::OK;
}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const P::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {
}

void MyPluginInterface::tool_on_press(const P::Vec2f &pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(const P::Vec2f &/*from*/, const P::Vec2f &to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(const P::Vec2f &/*pos*/) const {}

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

void slfn(P::Vec2f frac) {
    printf("frac is %f %f\n", frac.x, frac.y);
}

typedef void (*generate_triangle_type)(const P::Vec2f &pos, float radius, P::Vec2f &p1, P::Vec2f &p2, P::Vec2f &p3);

void MyPluginInterface::draw(const P::Vec2f &pos) const {
    // auto layout = APPI->factory.widget->window("opts", {pos, {400, 200}});
    // layout->set_name("abobs");

    // auto b1 = APPI->factory.widget->button({0, 0}, "TOUCH ME", layout);
    // b1->set_handler(func);
    
    // P::Vec2f p = b1->get_body().position;
    // p.x += b1->get_body().size.x;
    // p.y += b1->get_body().size.y;

    // auto b2 = APPI->factory.widget->slider(P::Slider::Type::X, {p, {100, 40}}, layout);
    // b2->set_base_color({100, 200, 255});
    // b2->set_handler(slfn);

    // p.x += b2->get_body().size.x;
    // p.y += b2->get_body().size.y;

    // auto picker = new Radio({p, {100, 100}}, layout);

    // layout->show();

    float size = APPI->get_size();
    P::RGBA color = APPI->get_color();  

    APPI->ext_enable("sharpy");
    auto func = (generate_triangle_type) APPI->ext_get_func("sharpy", "generate_triangle");

    P::Vec2f p0, p1, p2;
    func(pos, size, p0, p1, p2);

    auto target = APPI->get_target();
    
    target->render_triangle(p0, p1, p2, color, P::COPY);
    
    delete target;
}
