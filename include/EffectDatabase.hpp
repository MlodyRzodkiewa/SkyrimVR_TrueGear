#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

class TrueGearWebsocket
{
public:
    using client = websocketpp::client<websocketpp::config::asio_client>;
    using connection_ptr = client::connection_ptr;

    static TrueGearWebsocket& instance();

    void start();
    void stop();
    bool is_connected() const;

    void send_effect_json(const std::string& effectJsonBase64,
        const std::string& effectName);

    struct Effect
    {
        std::string name;
        std::string uuid;
        bool        keep;
        int         priority;
        std::vector<Track> tracks;

        std::string to_json() const;
    };

private:
    TrueGearWebsocket();
    ~TrueGearWebsocket();

    void run_loop();
    void connect();

    std::string make_play_body(const std::string& effectJsonBase64,
        const std::string& effectName);

    mutable std::mutex m_mutex;
    std::thread        m_thread;
    std::atomic<bool>  m_running{ false };
    std::atomic<bool>  m_connected{ false };

    client             m_client;
    websocketpp::connection_hdl m_hdl;
};
