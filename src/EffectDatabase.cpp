#include "EffectDatabase.hpp"
#include "json.hpp"

#include <fstream>
#include <iostream>
#include <windows.h>

using json = nlohmann::json;

static ActionType parse_action_type(const std::string& s);
static IntensityMode parse_intensity_mode(const std::string& s);

// WinAPI replacement for filesystem iteration
static std::vector<std::string> list_json_files(const std::string& folder) {
    std::vector<std::string> files;

    std::string pattern = folder + "\\*.json";
    WIN32_FIND_DATAA data;
    HANDLE hFind = FindFirstFileA(pattern.c_str(), &data);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            files.push_back(folder + "\\" + data.cFileName);
        } while (FindNextFileA(hFind, &data));

        FindClose(hFind);
    }
    return files;
}

std::string Effect::to_json() const
{
    json j;

    j["uuid"] = uuid;
    j["name"] = name;
    j["keep"] = keep ? "True" : "False";
    j["priority"] = priority;

    for (auto& tr : tracks) {
        json jt;
        jt["start_time"] = tr.start_time;
        jt["end_time"] = tr.end_time;
        jt["stop_name"] = tr.stop_name;
        jt["start_intensity"] = tr.start_intensity;
        jt["end_intensity"] = tr.end_intensity;
        jt["once"] = tr.once ? "True" : "False";
        jt["interval"] = tr.interval;

        // action_type / intensity_mode tak jak już miałeś (Shake/Electrical/Fade itd.)
        // + index jako tablica

        j["tracks"].push_back(jt);
    }

    return j.dump();
}



bool EffectDatabase::load_from_folder(const std::string& folder)
{
    effects.clear();

    auto files = list_json_files(folder);
    for (auto& path : files) {
        try {
            std::ifstream f(path);
            json j;
            f >> j;

            Effect e;
            e.name = j["name"].get<std::string>();
            e.uuid = j["uuid"].get<std::string>();
            e.keep = (j["keep"].get<std::string>() == "True");
            e.priority = j["priority"].get<int>();

            for (auto& t : j["tracks"]) {
                Track tr;

                tr.action_type = parse_action_type(t["action_type"].get<std::string>());
                tr.intensity_mode = parse_intensity_mode(t["intensity_mode"].get<std::string>());
                tr.once = (t["once"].get<std::string>() == "True");

                tr.interval = t["interval"].get<int>();
                tr.start_time = t["start_time"].get<int>();
                tr.end_time = t["end_time"].get<int>();
                tr.start_intensity = t["start_intensity"].get<int>();
                tr.end_intensity = t["end_intensity"].get<int>();
                tr.stop_name = t["stop_name"].get<std::string>();

                for (auto& idx : t["index"])
                    tr.index.push_back(idx.get<int>());

                e.tracks.push_back(tr);
            }

            // key = filename without extension
            size_t slash = path.find_last_of("\\/");
            size_t dot = path.find_last_of('.');
            std::string key = path.substr(slash + 1, dot - slash - 1);

            effects[key] = e;

            std::cout << "Loaded effect: " << key << "\n";
        }
        catch (std::exception& ex) {
            std::cout << "Error loading " << path << ": " << ex.what() << "\n";
        }
    }

    return !effects.empty();
}

const Effect* EffectDatabase::get(const std::string& name) const
{
    auto it = effects.find(name);
    if (it != effects.end())
        return &it->second;

    return nullptr;
}

static ActionType parse_action_type(const std::string& s) {
    if (s == "Shake") return ActionType::Shake;
    if (s == "Electrical") return ActionType::Electrical;
    throw std::runtime_error("Invalid action_type: " + s);
}

static IntensityMode parse_intensity_mode(const std::string& s) {
    if (s == "Const") return IntensityMode::Const;
    if (s == "Fade") return IntensityMode::Fade;
    if (s == "FadeInAndOut") return IntensityMode::FadeInAndOut;
    throw std::runtime_error("Invalid intensity_mode: " + s);
}
