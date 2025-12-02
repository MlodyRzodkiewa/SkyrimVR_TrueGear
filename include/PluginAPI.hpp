#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <utility>

#ifndef DLLEXPORT
#  define DLLEXPORT __declspec(dllexport)
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
    spdlog::set_level(spdlog::level::info);
}
