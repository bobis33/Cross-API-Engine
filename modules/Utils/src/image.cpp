#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Utils/Image.hpp"

utl::Image::Image(const std::string &path, const bool flip)
{
    if (flip != false)
    {
        stbi_set_flip_vertically_on_load(1);
    }
    pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (pixels == nullptr)
    {
        throw std::runtime_error("Failed to load image: " + path);
    }
}

utl::Image::~Image() { stbi_image_free(pixels); }
