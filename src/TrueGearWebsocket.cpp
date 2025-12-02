#include "TrueGearWebsocket.hpp"
#include "PluginAPI.hpp"

#include <spdlog/spdlog.h>

TrueGearWebsocket::TrueGearWebsocket() = default;

TrueGearWebsocket::~TrueGearWebsocket()
{
    Stop();
}

void TrueGearWebsocket::Start()
{
    db.load_from_folder("Data\\SKSE\\Plugins\\TrueGearEffects");
    connected = true;
    spdlog::info("[TrueGear] Websocket stub initialized");
}

void TrueGearWebsocket::Stop()
{
    connected = false;
}

void TrueGearWebsocket::PlayEffect(const std::string& name)
{
    if (!connected) {
        spdlog::warn("[TrueGear] Ignoring effect '{}' because websocket is not connected.", name);
        return;
    }

    const Effect* e = db.get(name);
    if (!e) {
        spdlog::warn("[TrueGear] Effect '{}' not found", name);
        return;
    }

    spdlog::info("[TrueGear] Playing effect '{}' (stub)", name);
}
