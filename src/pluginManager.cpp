#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <memory>

#include "CAE/PluginManager.hpp"
#include "Utils/Logger.hpp"


using EntryPointFn = std::unique_ptr<CAE::IPlugin>(*)();

CAE::PluginManager::~PluginManager() {
    for (const auto& path : m_handles | std::views::keys) {
        unloadPlugin(path);
    }
}

CAE::IPlugin* CAE::PluginManager::loadPlugin(const std::string& path) {
#ifdef _WIN32
    LibHandle handle = LoadLibraryA(path.c_str());
#else
    LibHandle handle = dlopen(path.c_str(), RTLD_LAZY);
#endif
    if (handle == nullptr) { throw std::runtime_error("Impossible to load plugin: " + path); }

#ifdef _WIN32
    auto entry = reinterpret_cast<EntryPointFn>(GetProcAddress(handle, "entryPoint"));
#else
    auto entry = reinterpret_cast<EntryPointFn>(dlsym(handle, "entryPoint"));
#endif
    if (entry == nullptr) { throw std::runtime_error("EntryPoint not found in plugin: " + path); }

    std::unique_ptr<IPlugin> plugin = entry();
    if (!plugin) { throw std::runtime_error("EntryPoint failed to create plugin instance: " + path); }

    IPlugin* rawPtr = plugin.get();

    m_handles[path] = handle;
    m_plugins[path] = std::move(plugin);

    utl::Logger::logInfo("Plugin loaded:\t name: " + std::string(rawPtr->getName()) + "\t type:" +
                         std::to_string(static_cast<uint8_t>(rawPtr->getType())) + "\t path: " + path);
    return rawPtr;
}

void CAE::PluginManager::unloadPlugin(const std::string& path) {
    auto it = m_plugins.find(path);
    if (it != m_plugins.end()) { m_plugins.erase(it); }

    if (auto itHandle = m_handles.find(path); itHandle != m_handles.end()) {
        utl::Logger::logInfo("Unloading plugin:\t path: " + path);

#ifdef _WIN32
        FreeLibrary(itHandle->second);
#else
        dlclose(itHandle->second);
#endif
        m_handles.erase(itHandle);
    }
}