#pragma once

#include <string>
#include <unordered_map>
#include <vector>

enum class ActionType
{
    Shake,
    Electrical
};

enum class IntensityMode
{
    Const,
    Fade,
    FadeInAndOut
};

struct Track
{
    ActionType action_type{};
    IntensityMode intensity_mode{};
    bool once{};
    int interval{};
    int start_time{};
    int end_time{};
    int start_intensity{};
    int end_intensity{};
    std::string stop_name;
    std::vector<int> index;
};

struct Effect
{
    std::string name;
    std::string uuid;
    bool keep{};
    int priority{};
    std::vector<Track> tracks;

    std::string to_json() const;
};

class EffectDatabase
{
public:
    bool load_from_folder(const std::string& folder);
    const Effect* get(const std::string& name) const;

private:
    std::unordered_map<std::string, Effect> effects;
};
