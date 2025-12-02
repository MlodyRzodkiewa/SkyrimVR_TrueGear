#pragma once

#include "EffectDatabase.hpp"
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

using client = websocketpp::client<websocketpp::config::asio_client>;

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

    // websocket internals
    client ws;
    websocketpp::connection_hdl hdl;
    bool connected = false;
    std::thread ws_thread;

    // effect loader
    EffectDatabase db;

    // handlers
    void OnOpen(client* c, websocketpp::connection_hdl h);
    void OnFail(client* c, websocketpp::connection_hdl h);
    void OnClose(client* c, websocketpp::connection_hdl h);
    void OnMessage(client* c, websocketpp::connection_hdl h, client::message_ptr msg);
};
