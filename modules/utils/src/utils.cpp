#include <fstream>

#include "Utils/Utils.hpp"

std::vector<char> utl::fileToVector(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file " + filename);
    }
    const long int fileSize = file.tellg();
    if (fileSize <= 0)
    {
        throw std::runtime_error("file " + filename + " is empty");
    }
    std::vector<char> buffer(static_cast<long unsigned int>(fileSize));
    file.seekg(0, std::ios::beg);
    if (!file.read(buffer.data(), fileSize))
    {
        throw std::runtime_error("failed to read file " + filename);
    }
    return buffer;
}

std::string utl::fileToString(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
