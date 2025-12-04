#pragma once

#include "EffectDatabase.hpp"
#include <string>

class TrueGearWebsocket
{
public:
    static TrueGearWebsocket& Get()
    {
        static TrueGearWebsocket inst;
        return inst;
    }

    bool IsConnected() const { return connected; }

    void Start();
    void Stop();
    void PlayEffect(const std::string& name);

private:
    TrueGearWebsocket();
    ~TrueGearWebsocket();

    bool connected = false;
    EffectDatabase db;
};
