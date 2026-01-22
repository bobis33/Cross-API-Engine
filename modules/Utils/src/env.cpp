#include "Utils/Env.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstring>

std::unordered_map<std::string, std::string> cae::utl::getEnvMap(const char *const *env)
{
    std::unordered_map<std::string, std::string> cpyEnv;

#ifdef _WIN32
    const LPCH envStrings = GetEnvironmentStringsA();
    if (envStrings == nullptr)
    {
        return cpyEnv;
    }

    for (LPCH var = envStrings; *var != 0; var += std::strlen(var) + 1)
    {
        std::string entry(var);
        if (const auto pos = entry.find('='); pos != std::string::npos)
        {
            cpyEnv.emplace(entry.substr(0, pos), entry.substr(pos + 1));
        }
    }

    FreeEnvironmentStringsA(envStrings);
#else
    for (const char *const *current = env; (current != nullptr) && (*current != nullptr); ++current)
    {
        std::string entry(*current);
        if (const auto pos = entry.find('='); pos != std::string::npos)
        {
            cpyEnv.emplace(entry.substr(0, pos), entry.substr(pos + 1));
        }
    }
#endif

    return cpyEnv;
}
