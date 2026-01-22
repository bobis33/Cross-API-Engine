#include "Utils/File.hpp"

#include <fstream>
#include <sstream>

std::vector<char> cae::utl::fileToVector(const std::filesystem::path &path)
{
    std::ifstream file(path, std::ios::in);
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file " + path.string());
    }
    const long int fileSize = file.tellg();
    if (fileSize <= 0)
    {
        throw std::runtime_error("file " + path.string() + " is empty");
    }
    std::vector<char> buffer(static_cast<long unsigned int>(fileSize));
    file.seekg(0, std::ios::beg);
    if (!file.read(buffer.data(), fileSize))
    {
        throw std::runtime_error("failed to read file " + path.string());
    }
    return buffer;
}

std::string cae::utl::fileToString(const std::filesystem::path &path)
{
    const std::ifstream file(path, std::ios::in);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + path.string());
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
