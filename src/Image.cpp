//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

#include "Image.hpp"
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <type_traits>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

template <typename DATA>
Image<DATA>::Image(const char *image_filename)
{
    // check if file exists
    std::error_code ec;
    std::filesystem::exists(image_filename, ec);

    if (!ec)
    {
        // read in image file
        if constexpr (!std::is_same<DATA, long>::value)
        {
            pixels = stbi_load(image_filename, &width, &height, &channels, 0);
            image_allocation = ImageAllocation::stbi_read;
        }
    }
    else
    {
        throw std::filesystem::filesystem_error(image_filename, ec);
    }
}

template <typename DATA>
Image<DATA>::Image(const int &w, const int &h, const int &c)
    : width(w), height(h), channels(c)
{
    if ((width < 1) || (height < 1) || (channels < 1))
    {
        throw std::invalid_argument("Dimensions won't form a real image");
    }
    pixels = new DATA[width * height * channels]();
    image_allocation = ImageAllocation::user_heap;
}

template <typename DATA>
Image<DATA> &Image<DATA>::operator=(const Image<DATA> &other_image)
{
    if (this != &other_image)
    {
        image_allocation = other_image.image_allocation;
        width = other_image.width;
        height = other_image.height;
        channels = other_image.channels;
        pixels = new DATA[width * height * channels];
        std::copy(other_image.pixels,
                  other_image.pixels + width * height * channels,
                  pixels);
    }
    return *this;
}

template <typename DATA>
Image<DATA>::~Image()
{
    if (image_allocation == ImageAllocation::stbi_read)
    {
        stbi_image_free(pixels);
    }
    else if (image_allocation == ImageAllocation::user_heap)
    {
        delete[] pixels;
    }
}

template <typename DATA>
const int &Image<DATA>::getWidth() const
{
    return width;
}

template <typename DATA>
const int &Image<DATA>::getHeight() const
{
    return height;
}

template <typename DATA>
const int &Image<DATA>::getChannels() const
{
    return channels;
}

template <typename DATA>
void Image<DATA>::setPixel(const DATA &value,
                           const int &x, const int &y, const int &ch)
{
    if (!((ch >= 0) && (ch < channels)))
    {
        throw std::out_of_range("Channel out of range");
    }
    else if ((x >= 0) && (x < width) && (y >= 0) && (y < height))
    {
        pixels[y * width * channels + x * channels + ch] = value;
    }
    else
    {
        throw std::out_of_range("Location out of range");
    }
}

template <typename DATA>
const DATA &Image<DATA>::getPixel(const int &x, const int &y, const int &ch,
                                  const PaddingType &pad,
                                  const DATA &constant_pad_value) const
{
    int x_new, y_new;
    if (!((ch >= 0) && (ch < channels)))
    {
        throw std::out_of_range("Channel out of range");
    }
    if ((x >= 0) && (x < width) && (y >= 0) && (y < height))
    {
        return pixels[y * width * channels + x * channels + ch];
    }
    else
    {
        switch (pad)
        {
        case PaddingType::constant:
            return constant_pad_value;
        case PaddingType::extend:
            x_new = std::max(0, std::min(x, width - 1));
            y_new = std::max(0, std::min(y, height - 1));
            return getPixel(x_new, y_new, ch);
        case PaddingType::reflect:
            x_new = reflect_into_domain(x, 0, width - 1);
            y_new = reflect_into_domain(y, 0, height - 1);
            return getPixel(x_new, y_new, ch);
        case PaddingType::wrap:
            x_new = wrap_into_domain(x, 0, width - 1);
            y_new = wrap_into_domain(y, 0, height - 1);
            return getPixel(x_new, y_new, ch);
        default:
            return constant_pad_value;
        }
    }
}

template <typename DATA>
const int Image<DATA>::reflect_into_domain(const int &num,
                                           const int &num_min, const int &num_max)
{
    if (num_min > num_max)
    {
        throw std::invalid_argument("Domain edges are incommensurate");
    }
    int num_reflected = num;
    while ((num_reflected < num_min) || (num_reflected > num_max))
    {
        if (num_reflected < num_min)
        {
            num_reflected = 2 * num_min - 1 - num_reflected;
        }
        if (num_reflected > num_max)
        {
            num_reflected = 2 * num_max + 1 - num_reflected;
        }
    }
    return num_reflected;
}

template <typename DATA>
const int Image<DATA>::wrap_into_domain(const int &num,
                                        const int &num_min, const int &num_max)
{
    if (num_min > num_max)
    {
        throw std::invalid_argument("Domain edges are incommensurate");
    }
    int num_reflected = num;
    while (num_reflected < num_min)
    {
        num_reflected += (num_max - num_min + 1);
    }
    while (num_reflected < num_min)
    {
        num_reflected -= (num_max - num_min + 1);
    }
    return num_reflected;
}

template <typename DATA>
void Image<DATA>::check_range_assign(std::vector<int> &output_vector,
                                     const std::vector<int> &input_vector,
                                     const int &range_min, const int &range_max)
{
    if (input_vector[0] == -1)
    {
        output_vector.front() = range_min;
        output_vector.back() = range_max;
    }
    else
    {
        for (std::size_t idx = 0; idx < 2; idx++)
        {
            if ((input_vector[idx] >= range_min) && (input_vector[idx] <= range_max))
            {
                output_vector[idx] = input_vector[idx];
            }
            else
            {
                throw std::out_of_range("Slice is out of range");
            }
        }
    }
}

template <typename DATA>
void Image<DATA>::write(const char *image_filename) const
{

    // save image to disk
    int success = stbi_write_png(image_filename, width, height, channels, pixels, 0);
    if (!success || !std::filesystem::exists(image_filename))
    {
        throw std::runtime_error("Image could not be saved to disk");
    }
}

// template instantiation
template class Image<unsigned char>;
template class Image<long>;
