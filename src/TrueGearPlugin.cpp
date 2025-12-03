#include "PluginAPI.hpp"
#include "TrueGearWebsocket.hpp"

using namespace SKSE;



namespace
{
    void ShowStartupNotification()
    {
        RE::ShowMessageBox("TrueGear VR initialised. Close to continue.");
    }

    void RegisterMCMTab()
    {
        MCM::RegisterPage("TrueGear VR", "Status");
    }

    // proste logowanie przez CommonLibVR
    void TG_Log(const char* msg)
    {
        logger::info("[TrueGear] {}", msg);
    }

    // wołane z Papyrusa: TrueGear.PlayEffect("arrow_hit")
    void TG_PlayEffect(RE::StaticFunctionTag*, RE::BSFixedString effectName)
    {
        logger::info("Papyrus requested effect '{}'.", effectName.c_str());
        TrueGearWebsocket::Get().PlayEffect(effectName.c_str());
    }

    // triggered after player drinks a potion
    void TG_OnPotionConsumed(RE::StaticFunctionTag*)
    {
        logger::info("Potion consumed -> triggering drink_potion haptic effect");
        TrueGearWebsocket::Get().PlayEffect("drink_potion");
    }

    // Rejestracja funkcji Papyrusa
    bool RegisterPapyrusFuncs(SKSE::PapyrusInterface* vm)
    {
        if (vm) {
            vm->Register(TG_PlayEffect);
            vm->Register(TG_OnPotionConsumed);
        }

        TG_Log("Papyrus functions registered.");
        return true;
    }
}

// Entry points required by SKSE
extern "C" DLLEXPORT bool SKSEPlugin_Query(const SKSEInterface*, PluginInfo* info)
{
    if (!info) {
        return false;
    }

    info->infoVersion = PluginInfo::kVersion;
    info->name = "TrueGear VR";
    info->version = 1;

    logger::info("SKSEPlugin_Query called; publishing plugin info.");
    return true;
}

extern "C" DLLEXPORT bool SKSEPlugin_Load(const LoadInterface* skse)
{
    Init(skse);

    // logger
    SetupLog(); // CommonLibVR template; jak nie masz, można zrobić ręcznie logger::init()

    logger::info("TrueGearPluginVR loaded.");

    ShowStartupNotification();

    // 1) start WebSocket
    TrueGearWebsocket::Get().Start();

    RegisterMCMTab();

    // 2) rejestracja Papyrusa
    auto papyrus = SKSE::GetPapyrusInterface();
    RegisterPapyrusFuncs(papyrus);

    return true;
}
