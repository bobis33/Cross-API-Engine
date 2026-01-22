#include "Utils/Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

cae::utl::Image::Image(const std::filesystem::path &path, const bool flip)
{
    if (flip)
    {
        stbi_set_flip_vertically_on_load(1);
    }
    pixels = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (pixels == nullptr)
    {
        throw std::runtime_error("Failed to load image: " + path.string());
    }
}

cae::utl::Image::~Image() { stbi_image_free(pixels); }
