#include <cstring>
#include <string>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Utils/Env.hpp"

std::unordered_map<std::string, std::string> utl::getEnvMap(const char *const *env)
{
    std::unordered_map<std::string, std::string> cpyEnv;

#ifdef _WIN32
    LPCH envStrings = GetEnvironmentStringsA();
    if (!envStrings)
    {
        return cpyEnv;
    }

    for (LPCH var = envStrings; *var; var += std::strlen(var) + 1)
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
