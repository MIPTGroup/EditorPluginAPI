#include "redactor/plugin_std/std.hpp"


struct MyPluginInterface : public P::PluginInterface {
    bool  ext_enable        (const char */*name*/)                            const override;
    void *ext_get_func      (const char */*extension*/, const char */*func*/) const override;
    void *ext_get_interface (const char */*extension*/, const char */*name*/) const override;

    const P::PluginInfo *get_info() const override;

    P::Status init   (const P::AppInterface*) const override;
    P::Status deinit ()                       const override;
    void      dump   ()                       const override;

    void on_tick(double dt)   const override;

    void effect_apply() const override;

    void tool_on_press  (const P::Vec2f &position)                 const override;
    void tool_on_release(const P::Vec2f &position)                 const override;
    void tool_on_move   (const P::Vec2f &from, const P::Vec2f &to) const override;

// settings
    void show_settings() const override;

//additional
    void draw(const P::Vec2f &position) const;

    P::Extensioner exts;
};

bool  MyPluginInterface::ext_enable        (const char */*name*/)                        const { return false;   }
void *MyPluginInterface::ext_get_func      (const char */*extension*/, const char *func) const { return exts.get(func); }
void *MyPluginInterface::ext_get_interface (const char */*extension*/, const char *name) const { return exts.get(name); }

