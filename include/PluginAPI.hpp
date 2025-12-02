#pragma once

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <exception>
#include <string>
#include <string_view>
#include <utility>

#ifndef DLLEXPORT
#  if defined(_MSC_VER)
#    define DLLEXPORT __declspec(dllexport)
#  else
#    define DLLEXPORT __attribute__((visibility("default")))
#  endif
#endif

struct SKSEInterface {};
struct LoadInterface {};

namespace RE
{
    struct StaticFunctionTag
    {
    };

    class BSFixedString
    {
    public:
        BSFixedString() = default;
        explicit BSFixedString(const char* value) : data(value) {}

        const char* c_str() const { return data.c_str(); }

    private:
        std::string data;
    };
}

namespace SKSE
{
    inline void Init(const LoadInterface*) {}

    struct PapyrusInterface
    {
        template <class Fn>
        bool Register(Fn&&) const
        {
            return true;
        }
    };

    inline PapyrusInterface* GetPapyrusInterface()
    {
        static PapyrusInterface inst;
        return &inst;
    }
}

namespace logger
{
    template <class... Args>
    void info(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        spdlog::info(fmt_str, std::forward<Args>(args)...);
    }

    template <class... Args>
    void warn(fmt::format_string<Args...> fmt_str, Args&&... args)
    {
        spdlog::warn(fmt_str, std::forward<Args>(args)...);
    }
}

inline void SetupLog()
{
    try {
        const auto log_path = std::filesystem::path("Data/SKSE/Plugins/TrueGear.log");
        std::filesystem::create_directories(log_path.parent_path());

        auto file_logger = spdlog::basic_logger_mt("TrueGearLog", log_path.string(), true);
        file_logger->set_level(spdlog::level::info);
        file_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        spdlog::set_default_logger(file_logger);
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to setup file logger: {}", e.what());
    }
}

namespace RE
{
    inline void ShowMessageBox(const std::string_view msg)
    {
        spdlog::info("[Popup] {}", msg);
    }
}

namespace MCM
{
    inline void RegisterPage(const std::string_view mod_name, const std::string_view page_name)
    {
        spdlog::info("[MCM] Registered page '{}' for '{}'.", page_name, mod_name);
    }
}
