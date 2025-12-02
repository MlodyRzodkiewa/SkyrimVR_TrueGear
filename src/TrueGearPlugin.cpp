#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include "TrueGearWebsocket.hpp"
#include "EffectDatabase.hpp"

using namespace SKSE;



namespace
{
    std::unique_ptr<EffectDatabase> g_effects;

    extern "C" DLLEXPORT bool SKSEPlugin_Load(const SKSEInterface* skse)
    {
        // Start WebSocket client for TrueGear
        TrueGearWebsocket::Get().Start();

        _MESSAGE("TrueGear Websocket started");

        return true;
    }


    // proste logowanie przez CommonLibVR
    void TG_Log(const char* msg)
    {
        logger::info("[TrueGear] {}", msg);
    }

    // wołane z Papyrusa: TrueGear.PlayEffect("arrow_hit")
    void TG_PlayEffect(RE::StaticFunctionTag*, RE::BSFixedString effectName)
    {
        if (!g_effects) {
            TG_Log("Effect DB not initialized!");
            return;
        }

        const Effect* eff = g_effects->get(effectName.c_str());
        if (!eff) {
            logger::warn("[TrueGear] Effect '{}' not found", effectName.c_str());
            return;
        }

        std::string rawJson = eff->to_json();      // zaraz pokażę jak to dodać
        std::string encoded = base64_encode(rawJson); // też już masz

        TrueGearWebsocket::instance().send_effect_json(encoded, eff->name);
    }

    // Rejestracja funkcji Papyrusa
    bool RegisterPapyrusFuncs(RE::BSScript::IVirtualMachine* vm)
    {
        vm->RegisterFunction(
            "PlayEffect",
            "TrueGear",
            TG_PlayEffect
        );

        TG_Log("Papyrus functions registered.");
        return true;
    }
}

// Entry point pluginu
SKSEPluginLoad(const LoadInterface* skse)
{
    Init(skse);

    // logger
    SetupLog(); // CommonLibVR template; jak nie masz, można zrobić ręcznie logger::init()

    logger::info("TrueGearPluginVR loaded.");

    // 1) start WebSocket
    TrueGearWebsocket::instance().start();

    // 2) wczytaj efekty z folderu "effects"
    g_effects = std::make_unique<EffectDatabase>();
    g_effects->load_from_folder("Data\\SKSE\\Plugins\\TrueGear\\effects");

    // 3) rejestracja Papyrusa
    auto papyrus = GetPapyrusInterface();
    papyrus->Register(RegisterPapyrusFuncs);

    return true;
}
