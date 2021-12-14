#include "redactor/plugin_std/plugin_std.hpp"
#include "plugin_interface.h"
#include "abstract_widget.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

// ============================================================================ Info

const auto PTYPE = PUPPY::TOOL; // PUPPY::EFFECT | PUPPY::EXTENSION

const char *PNAME    = "a_name";
const char *PVERSION = "a_vesion";
const char *PAUTHOR  = "an_author";
const char *PDESCR   = "a_description";

// ============================================================================ Resources

// ============================================================================ General

const MyPluginInterface PINTERFACE;

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

PUPPY::Status MyPluginInterface::init(const PUPPY::AppInterface *app_interface) const {
    srand(time(NULL));

    APPI = app_interface;

    APPI->log("[plugin](%s) inited", PINFO.name);
    return PUPPY::OK;
}

PUPPY::Status MyPluginInterface::deinit() const {
    APPI->log("[plugin](%s) deinited | %s thanks you for using it", PINFO.name, PINFO.author);
    return PUPPY::OK;
}

void MyPluginInterface::show_settings() const {}

void MyPluginInterface::dump() const {
    APPI->log("[plugin](%s) is active", PINFO.name);
}

const PUPPY::PluginInfo *MyPluginInterface::get_info() const {
    return &PINFO;
}

void MyPluginInterface::on_tick(double /*dt*/) const {}

void MyPluginInterface::tool_on_press(const PUPPY::Vec2f &pos) const {
    draw(pos);
}

void MyPluginInterface::tool_on_move(const PUPPY::Vec2f &/*from*/, const PUPPY::Vec2f &to) const {
    draw(to);
}

void MyPluginInterface::tool_on_release(const PUPPY::Vec2f &/*pos*/) const {}

void MyPluginInterface::effect_apply() const {}

void MyPluginInterface::draw(const PUPPY::Vec2f &pos) const {
    float    size = APPI->get_size();
    PUPPY::RGBA color = APPI->get_color();

    auto preview = APPI->get_preview();

    preview->render_circle(pos, size, color, PUPPY::COPY);

    delete preview;
}
