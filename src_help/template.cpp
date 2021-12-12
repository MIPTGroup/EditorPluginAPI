#include "plugin_std.hpp"
#include "an_interface.h"
#include "a_widget.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = P::TOOL; // P::EFFECT | P::EXTENSION

const char *PNAME    = "a_name";
const char *PVERSION = "a_vesion";
const char *PAUTHOR  = "an_author";
const char *PDESCR   = "a_description";

// ============================================================================ Resources

// ============================================================================ General

const MyPluginInterface PINTERFACE;

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

P::Status MyPluginInterface::init(const P::AppInterface *app_interface) const {
    srand(time(NULL));

    APPI = app_interface;

    APPI->log("[plugin](%s) inited", PINFO.name);
    return P::OK;
}

P::Status MyPluginInterface::deinit() const {
    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return P::OK;
}

void MyPluginInterface::show_settings() const {}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const P::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {}

void MyPluginInterface::tool_on_press(const P::Vec2f &pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(const P::Vec2f &/*from*/, const P::Vec2f &to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(const P::Vec2f &/*pos*/) const {}

void MyPluginInterface::effect_apply() const {}

void MyPluginInterface::draw(const P::Vec2f &pos) const {
    float    size = APPI->get_size();
    P::RGBA color = APPI->get_color();

    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, P::COPY);

    delete preview;
}
