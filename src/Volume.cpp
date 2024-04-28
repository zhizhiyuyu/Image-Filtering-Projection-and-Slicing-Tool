//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

#include <iomanip>
#include <numeric>
#include <stdexcept>
#include <sstream>
#include <vector>
#include "Volume.hpp"
#include "Image.hpp"

// constructor delegation
template <typename DATA>
Volume<DATA>::Volume(const char *image_filename_prefix,
                     const int &index_begin, const int &index_end,
                     const std::size_t &field_width,
                     const char *image_filename_suffix)
    : Volume<DATA>(image_filename_prefix,
                   return_iota(index_begin, index_end),
                   field_width,
                   image_filename_suffix)
{
}

template <typename DATA>
Volume<DATA>::Volume(const char *image_filename_prefix,
                     const std::vector<int> &index_vector,
                     const std::size_t &field_width,
                     const char *image_filename_suffix)
    : image_indices(index_vector)
{
    std::stringstream image_filename;
    for (const int &index : image_indices)
    {
        // create filenames and load from disk
        image_filename.str("");
        image_filename << image_filename_prefix
                       << std::setw(field_width) << std::setfill('0') << index
                       << image_filename_suffix;
        image_slices.push_back(new Image<DATA>(image_filename.str().c_str()));
    }

    // check dimension matching
    depth = static_cast<int>(image_slices.size());
    width = image_slices[0]->getWidth();
    height = image_slices[0]->getHeight();
    channels = image_slices[0]->getChannels();
    for (const auto &slice : image_slices)
    {
        if ((slice->getWidth() != width) || (slice->getHeight() != height) || (slice->getChannels() != channels))
        {
            throw std::invalid_argument("All slices must have the same dimensions");
        }
    }
}

template <typename DATA>
Volume<DATA>::Volume(const int &d, const int &w, const int &h, const int &c)
    : depth(d), width(w), height(h), channels(c)
{
    if ((depth < 1) || (width < 1) || (height < 1) || (channels < 1))
    {
        throw std::invalid_argument("Dimensions won't form a real volume");
    }
    for (int idx = 0; idx < depth; idx++)
    {
        image_slices.push_back(new Image<DATA>(width, height, channels));
    }
}

template <typename DATA>
Volume<DATA>::~Volume()
{
    for (const auto &slice : image_slices)
    {
        delete slice;
    }
}

template <typename DATA>
std::vector<int> Volume<DATA>::return_iota(const int &begin, const int &end)
{
    std::vector<int> temp_vector;
    temp_vector.resize(end - begin + 1);
    std::iota(temp_vector.begin(), temp_vector.end(), begin);
    return temp_vector;
}

template <typename DATA>
const int &Volume<DATA>::getDepth() const
{
    return depth;
}

template <typename DATA>
const int &Volume<DATA>::getWidth() const
{
    return width;
}

template <typename DATA>
const int &Volume<DATA>::getHeight() const
{
    return height;
}

template <typename DATA>
const int &Volume<DATA>::getChannels() const
{
    return channels;
}

template <typename DATA>
const std::vector<int> &Volume<DATA>::getIndices() const
{
    return image_indices;
}

template <typename DATA>
void Volume<DATA>::setIndices(const std::vector<int> &arg_indices)
{
    image_indices = arg_indices;
}

template <typename DATA>
void Volume<DATA>::setVoxel(const DATA &value,
                            const int &z, const int &x, const int &y, const int &ch)
{
    if (!((ch >= 0) && (ch < channels)))
    {
        throw std::out_of_range("Channel out of range");
    }
    else if ((z >= 0) && (z < depth) && (x >= 0) && (x < width) && (y >= 0) && (y < height))
    {
        image_slices[z]->setPixel(value, x, y, ch);
    }
    else
    {
        throw std::out_of_range("Location out of range");
    }
}

template <typename DATA>
const DATA &Volume<DATA>::getVoxel(const int &z, const int &x, const int &y, const int &ch,
                                   const PaddingType &pad,
                                   const DATA &constant_pad_value) const
{
    int z_new;
    if (!((ch >= 0) && (ch < channels)))
    {
        throw std::out_of_range("Channel out of range");
    }
    if ((z >= 0) && (z < depth))
    {
        return image_slices[z]->getPixel(x, y, ch, pad, constant_pad_value);
    }
    else
    {
        switch (pad)
        {
        case PaddingType::constant:
            return constant_pad_value;
        case PaddingType::extend:
            z_new = std::max(0, std::min(z, depth - 1));
            return image_slices[z_new]->getPixel(x, y, ch, PaddingType::extend);
        case PaddingType::reflect:
            z_new = Image<DATA>::reflect_into_domain(z, 0, depth - 1);
            return image_slices[z_new]->getPixel(x, y, ch, PaddingType::reflect);
        case PaddingType::wrap:
            z_new = Image<DATA>::wrap_into_domain(z, 0, depth - 1);
            return image_slices[z_new]->getPixel(x, y, ch, PaddingType::wrap);
        default:
            return constant_pad_value;
        }
    }
}

template <typename DATA>
void Volume<DATA>::write(const char *image_filename_prefix,
                         const std::size_t &field_width,
                         const char *image_filename_suffix) const
{
    std::stringstream image_filename;
    auto slice_iterator = image_slices.begin();
    for (const int &index : image_indices)
    {
        // save images to disk
        image_filename.str("");
        image_filename << image_filename_prefix
                       << std::setw(field_width) << std::setfill('0') << index
                       << image_filename_suffix;
        (*slice_iterator)->write(image_filename.str().c_str());
        slice_iterator++;
    }
}

// template instantiation
template class Volume<unsigned char>;