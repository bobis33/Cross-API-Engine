#include "CAE/Application.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

namespace fs = std::filesystem;
using json = nlohmann::json;

cae::EngineConfig cae::Application::parseEngineConf(const std::string &path)
{
    const fs::path filePath(path);
    if (!fs::exists(filePath))
    {
        std::cerr << "Config file not found: " << filePath << '\n';
        return {};
    }
    if (!fs::is_regular_file(filePath))
    {
        std::cerr << "Config path is not a regular file: " << filePath << '\n';
        return {};
    }

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open config file: " << filePath << '\n';
        return {};
    }

    json j;
    try
    {
        file >> j;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "Failed to parse JSON config (" << filePath << "): " + std::string(e.what()) << '\n';
        return {};
    }
    EngineConfig config;
    utl::Logger::log("Loading config: " + filePath.string(), utl::LogLevel::INFO);
    if (j.contains("audio"))
    {
        const auto &audio = j["audio"];
        if (audio.contains("masterVolume") && audio["masterVolume"].is_number_float())
        {
            config.audio_master_volume = audio["masterVolume"];
        }
        if (audio.contains("muted") && audio["muted"].is_boolean())
        {
            config.audio_muted = audio["muted"];
        }
    }
    if (j.contains("network"))
    {
        const auto &network = j["network"];
        if (network.contains("host") && network["host"].is_string())
        {
            config.network_host = network["host"];
        }
        if (network.contains("port") && network["port"].is_number_unsigned())
        {
            config.network_port = network["port"];
        }
    }
    if (j.contains("renderer"))
    {
        const auto &renderer = j["renderer"];
        if (renderer.contains("vsync") && renderer["vsync"].is_boolean())
        {
            config.renderer_vsync = renderer["vsync"];
        }
        if (renderer.contains("frameRateLimit") && renderer["frameRateLimit"].is_number_unsigned())
        {
            config.renderer_frame_rate_limit = renderer["frameRateLimit"];
        }
        if (renderer.contains("clearColor") && renderer["clearColor"].is_array() && renderer["clearColor"].size() == 4)
        {
            if (const auto &clearColor = renderer["clearColor"]; clearColor[0].is_number_float() &&
                                                                 clearColor[1].is_number_float() &&
                                                                 clearColor[2].is_number_float() && clearColor[3].is_number_float())
            {
                config.renderer_clear_color.r = clearColor[0];
                config.renderer_clear_color.g = clearColor[1];
                config.renderer_clear_color.b = clearColor[2];
                config.renderer_clear_color.a = clearColor[3];
            }
        }
    }
    if (j.contains("window"))
    {
        const auto &window = j["window"];
        if (window.contains("width") && window["width"].is_number_unsigned())
        {
            config.window_width = window["width"];
        }
        if (window.contains("height") && window["height"].is_number_unsigned())
        {
            config.window_height = window["height"];
        }
        if (window.contains("fullscreen") && window["fullscreen"].is_boolean())
        {
            config.window_fullscreen = window["fullscreen"];
        }
        if (window.contains("name") && window["name"].is_string())
        {
            config.window_name = window["name"];
        }
        if (window.contains("iconPath") && window["iconPath"].is_string())
        {
            config.window_icon_path = window["iconPath"];
        }
    }

    return config;
}
