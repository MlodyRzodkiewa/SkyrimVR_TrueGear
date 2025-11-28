#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace {

std::string base64_encode(const std::string &input) {
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encoded;
    encoded.reserve(((input.size() + 2) / 3) * 4);

    uint32_t buffer = 0;
    int bits_collected = 0;

    for (unsigned char c : input) {
        buffer = (buffer << 8) | c;
        bits_collected += 8;
        while (bits_collected >= 6) {
            bits_collected -= 6;
            encoded.push_back(table[(buffer >> bits_collected) & 0x3F]);
        }
    }

    if (bits_collected > 0) {
        buffer <<= 6 - bits_collected;
        encoded.push_back(table[buffer & 0x3F]);
    }

    while (encoded.size() % 4 != 0) {
        encoded.push_back('=');
    }

    return encoded;
}

std::string make_sample_effect_body() {
    const std::string effect = R"({
        "name": "CppQuickBuzz",
        "uuid": "CppQuickBuzz",
        "keep": "False",
        "priority": 0,
        "tracks": [
            {
                "start_time": 0,
                "end_time": 120,
                "stop_name": "",
                "start_intensity": 60,
                "end_intensity": 20,
                "intensity_mode": "Fade",
                "action_type": "Shake",
                "once": "False",
                "interval": 0,
                "index": [0, 4, 8]
            }
        ]
    })";

    return base64_encode(effect);
}

}  // namespace

int main(int argc, char *argv[]) {
    using client = websocketpp::client<websocketpp::config::asio_client>;

    std::string ws_uri = "ws://127.0.0.1:18233/v1/tact/";
    if (argc > 1) {
        ws_uri = argv[1];
    }

    client c;
    c.clear_access_channels(websocketpp::log::alevel::all);
    c.clear_error_channels(websocketpp::log::elevel::all);
    c.init_asio();

    std::atomic<bool> connected{false};
    websocketpp::connection_hdl hdl;

    c.set_open_handler([&](websocketpp::connection_hdl handle) {
        connected = true;
        hdl = handle;
        std::cout << "Connected to TrueGear server at " << ws_uri << "\n";
        std::cout << "Press ENTER to fire a short buzz, or type 'quit' to exit." << std::endl;
    });

    c.set_close_handler([&](websocketpp::connection_hdl) {
        connected = false;
        std::cout << "Connection closed." << std::endl;
    });

    c.set_fail_handler([&](websocketpp::connection_hdl) {
        connected = false;
        std::cerr << "Connection failed. Check that Truegear_player is running and the URL is correct." << std::endl;
    });

    c.set_message_handler([&](websocketpp::connection_hdl, client::message_ptr msg) {
        std::cout << "Server message: " << msg->get_payload() << std::endl;
    });

    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(ws_uri, ec);
    if (ec) {
        std::cerr << "Could not create connection: " << ec.message() << std::endl;
        return 1;
    }

    c.connect(con);

    std::thread runner([&]() { c.run(); });

    const std::string payload = std::string("{\"Method\":\"play_no_registered\",\"Body\":\"") +
                                 make_sample_effect_body() + "\"}";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "quit" || line == "exit") {
            break;
        }

        if (!connected) {
            std::cout << "Not connected yet. Waiting..." << std::endl;
            continue;
        }

        websocketpp::lib::error_code send_ec;
        c.send(hdl, payload, websocketpp::frame::opcode::text, send_ec);
        if (send_ec) {
            std::cerr << "Failed to send: " << send_ec.message() << std::endl;
        } else {
            std::cout << "Triggered CppQuickBuzz on motors 0,4,8." << std::endl;
        }
    }

    if (connected) {
        websocketpp::lib::error_code close_ec;
        c.close(hdl, websocketpp::close::status::normal, "Client shutdown", close_ec);
        if (close_ec) {
            std::cerr << "Failed to close cleanly: " << close_ec.message() << std::endl;
        }
    }

    c.stop_perpetual();
    runner.join();

    return 0;
}
