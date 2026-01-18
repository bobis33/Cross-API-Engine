#include "CAE/Application.hpp"

#include "Utils/Logger.hpp"
#include "Utils/Path.hpp"

#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

cae::EngineConfig cae::Application::parseEngineConf(const std::string &path)
{
    auto confPath = utl::Path::resolveRelativeToExe(path);
    if (!utl::Path::existsFile(confPath))
    {
        utl::Logger::log("Config file does not exist: " + confPath.string(), utl::LogLevel::WARNING);
        return {};
    }

    std::ifstream file(confPath);
    if (!file.is_open())
    {
        utl::Logger::log("Failed to open config file: " + confPath.string(), utl::LogLevel::WARNING);
        return {};
    }

    json j;
    try
    {
        file >> j;
    }
    catch (const json::parse_error &e)
    {
        utl::Logger::log("Failed to parse JSON config (" + confPath.string() + "): " + std::string(e.what()),
                         utl::LogLevel::WARNING);
        return {};
    }
    EngineConfig config;
    utl::Logger::log("Loading config: " + confPath.string(), utl::LogLevel::INFO);
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
    if (j.contains("camera"))
    {
        const auto &camera = j["camera"];
        if (camera.contains("position") && camera["position"].is_array() && camera["position"].size() == 3)
        {
            if (const auto &position = camera["position"];
                position[0].is_number_float() && position[1].is_number_float() && position[2].is_number_float())
            {
                config.camera_position.x = position[0];
                config.camera_position.y = position[1];
                config.camera_position.z = position[2];
            }
        }
        if (camera.contains("rotation") && camera["rotation"].is_array() && camera["rotation"].size() == 3)
        {
            if (const auto &rotation = camera["rotation"];
                rotation[0].is_number_float() && rotation[1].is_number_float() && rotation[2].is_number_float())
            {
                config.camera_rotation.x = rotation[0];
                config.camera_rotation.y = rotation[1];
                config.camera_rotation.z = rotation[2];
            }
        }
        if (camera.contains("direction") && camera["direction"].is_array() && camera["direction"].size() == 3)
        {
            if (const auto &direction = camera["direction"];
                direction[0].is_number_float() && direction[1].is_number_float() && direction[2].is_number_float())
            {
                config.camera_direction.x = direction[0];
                config.camera_direction.y = direction[1];
                config.camera_direction.z = direction[2];
            }
        }
        if (camera.contains("movementSpeed") && camera["movementSpeed"].is_number_float())
        {
            config.camera_move_speed = camera["movementSpeed"];
        }
        if (camera.contains("rotationSpeed") && camera["rotationSpeed"].is_number_float())
        {
            config.camera_look_speed = camera["rotationSpeed"];
        }
        if (camera.contains("fov") && camera["fov"].is_number_unsigned())
        {
            config.camera_fov = camera["fov"];
        }
        if (camera.contains("nearPlane") && camera["nearPlane"].is_number_float())
        {
            config.camera_near_plane = camera["nearPlane"];
        }
        if (camera.contains("farPlane") && camera["farPlane"].is_number_float())
        {
            config.camera_far_plane = camera["farPlane"];
        }
    }
    if (j.contains("log"))
    {
        if (const auto &log = j["log"]; log.contains("fps") && log["fps"].is_boolean())
        {
            config.log_fps = log["fps"];
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
            if (const auto &clearColor = renderer["clearColor"];
                clearColor[0].is_number_float() && clearColor[1].is_number_float() && clearColor[2].is_number_float() &&
                clearColor[3].is_number_float())
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
            config.window_icon_path = utl::Path::resolveRelativeToExe(window["iconPath"]).string();
        }
    }

    return config;
}
