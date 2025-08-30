#pragma once

namespace utl
{

#define THROW_ERROR(msg) std::runtime_error(std::string(msg) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")")

    void printError(const std::string &msg);

} // namespace utl
