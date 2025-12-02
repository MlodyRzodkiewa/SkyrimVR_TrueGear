#include "TrueGearWebsocket.hpp"
#include <Windows.h>
#include <wincrypt.h>
#include "json.hpp"

#pragma comment(lib, "crypt32.lib")
using json = nlohmann::json;

static std::string Base64(const std::string& input)
{
    DWORD out_len = 0;

    CryptBinaryToStringA(
        reinterpret_cast<const BYTE*>(input.data()),
        (DWORD)input.size(),
        CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        nullptr, &out_len
    );

    std::string out(out_len, '\0');

    CryptBinaryToStringA(
        reinterpret_cast<const BYTE*>(input.data()),
        (DWORD)input.size(),
        CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        out.data(), &out_len
    );

    if (!out.empty() && out.back() == '\0')
        out.pop_back();

    return out;
}

TrueGearWebsocket::TrueGearWebsocket()
{
}

TrueGearWebsocket::~TrueGearWebsocket()
{
    Stop();
}

void TrueGearWebsocket::Start()
{
    // Load effects
    db.load_from_folder("Data\\SKSE\\Plugins\\TrueGearEffects");

    ws.set_access_channels(websocketpp::log::alevel::none);
    ws.set_error_channels(websocketpp::log::elevel::none);

    ws.init_asio();

    ws.set_open_handler([this](auto h) { OnOpen(&ws, h); });
    ws.set_fail_handler([this](auto h) { OnFail(&ws, h); });
    ws.set_close_handler([this](auto h) { OnClose(&ws, h); });
    ws.set_message_handler([this](auto h, auto m) { OnMessage(&ws, h, m); });

    websocketpp::lib::error_code ec;
    auto con = ws.get_connection("ws://127.0.0.1:18233/v1/tact/", ec);

    ws.connect(con);

    ws_thread = std::thread([this]() {
        ws.run();
        });
}

void TrueGearWebsocket::Stop()
{
    if (ws.stopped() == false)
        ws.stop();

    if (ws_thread.joinable())
        ws_thread.join();
}

void TrueGearWebsocket::OnOpen(client* c, websocketpp::connection_hdl h)
{
    connected = true;
    hdl = h;
}

void TrueGearWebsocket::OnFail(client* c, websocketpp::connection_hdl h)
{
    connected = false;
}

void TrueGearWebsocket::OnClose(client* c, websocketpp::connection_hdl h)
{
    connected = false;
}

void TrueGearWebsocket::OnMessage(client* c, websocketpp::connection_hdl, client::message_ptr msg)
{
    _MESSAGE("[TrueGear] WS RECV %s", msg->get_payload().c_str());
}

void TrueGearWebsocket::PlayEffect(const std::string& name)
{
    if (!connected)
        return;

    const Effect* e = db.get(name);
    if (!e)
        return;

    json j;
    j["name"] = e->name;
    j["uuid"] = e->uuid;
    j["keep"] = e->keep ? "True" : "False";
    j["priority"] = e->priority;
    j["tracks"] = json::array();

    for (auto& t : e->tracks)
    {
        json jt;
        jt["action_type"] = (t.action_type == ActionType::Shake ? "Shake" : "Electrical");
        jt["intensity_mode"] =
            (t.intensity_mode == IntensityMode::Const ? "Const" :
                t.intensity_mode == IntensityMode::Fade ? "Fade" : "FadeInAndOut");

        jt["once"] = t.once ? "True" : "False";
        jt["interval"] = t.interval;
        jt["start_time"] = t.start_time;
        jt["end_time"] = t.end_time;
        jt["start_intensity"] = t.start_intensity;
        jt["end_intensity"] = t.end_intensity;
        jt["stop_name"] = t.stop_name;
        jt["index"] = t.index;

        j["tracks"].push_back(jt);
    }

    json msg;
    msg["Method"] = "play_no_registered";
    msg["Body"] = Base64(j.dump());

    ws.send(hdl, msg.dump(), websocketpp::frame::opcode::text);
}
