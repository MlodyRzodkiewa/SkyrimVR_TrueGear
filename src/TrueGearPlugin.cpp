#include "PluginAPI.hpp"
#include "TrueGearWebsocket.hpp"

using namespace SKSE;



namespace
{
    extern "C" DLLEXPORT bool SKSEPlugin_Load(const SKSEInterface* skse)
    {
        // Start WebSocket client for TrueGear
        TrueGearWebsocket::Get().Start();

        logger::info("TrueGear Websocket started");

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
        TrueGearWebsocket::Get().PlayEffect(effectName.c_str());
    }

    // Rejestracja funkcji Papyrusa
    bool RegisterPapyrusFuncs(SKSE::PapyrusInterface* vm)
    {
        if (vm) {
            vm->Register(TG_PlayEffect);
        }

        TG_Log("Papyrus functions registered.");
        return true;
    }
}

// Entry point pluginu
extern "C" DLLEXPORT bool SKSEPluginLoad(const LoadInterface* skse)
{
    Init(skse);

    // logger
    SetupLog(); // CommonLibVR template; jak nie masz, można zrobić ręcznie logger::init()

    logger::info("TrueGearPluginVR loaded.");

    // 1) start WebSocket
    TrueGearWebsocket::Get().Start();

    // 2) rejestracja Papyrusa
    auto papyrus = SKSE::GetPapyrusInterface();
    RegisterPapyrusFuncs(papyrus);

    return true;
}
