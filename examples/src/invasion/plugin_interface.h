#include "redactor/plugin_std/plugin_std.hpp"


struct MyPluginInterface : public PUPPY::PluginInterface {
    bool  ext_enable        (const char */*name*/)                            const override;
    void *ext_get_func      (const char */*extension*/, const char */*func*/) const override;
    void *ext_get_interface (const char */*extension*/, const char */*name*/) const override;

    const PUPPY::PluginInfo *get_info() const override;

    PUPPY::Status init   (const PUPPY::AppInterface*, const std::filesystem::path& path = std::filesystem::path("./")) override;
    PUPPY::Status deinit ()                                                                                            override;
    
    void dump() const override;

    void on_tick(double dt)   const override;

    void effect_apply() const override;

    void tool_on_press  (const PUPPY::Vec2f &position)                 const override;
    void tool_on_release(const PUPPY::Vec2f &position)                 const override;
    void tool_on_move   (const PUPPY::Vec2f &from, const PUPPY::Vec2f &to) const override;

// settings
    void show_settings() const override;

//additional
    void draw(const PUPPY::Vec2f &position) const;

    PUPPY::Extensioner exts;
};

bool  MyPluginInterface::ext_enable        (const char */*name*/)                        const { return false;   }
void *MyPluginInterface::ext_get_func      (const char */*extension*/, const char *func) const { return exts.get(func); }
void *MyPluginInterface::ext_get_interface (const char */*extension*/, const char *name) const { return exts.get(name); }


MyPluginInterface PINTERFACE {};

const PUPPY::PluginInfo PINFO =
{
    PUPPY::STD_VERSION, // std_version
    nullptr,            // reserved

    &PINTERFACE,

    PNAME,
    PVERSION,
    PAUTHOR,
    PDESCR,

    #ifdef ICON // icon
    ICON,
    #else
    nullptr,
    #endif

    PTYPE
};

const PUPPY::AppInterface *APPI = nullptr;


extern "C" PUPPY::PluginInterface *get_plugin_interface() {
    return &PINTERFACE;
}