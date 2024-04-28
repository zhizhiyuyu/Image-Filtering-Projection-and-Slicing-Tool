//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "Filter.hpp"
#include "Image.hpp"
#include "Volume.hpp"
#include "Quickselect.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>

// Filter2D definitions
// identity transform as base class
template <typename WORK, typename STORE>
WORK Filter2D<WORK, STORE>::applyPerElement(const Image<STORE> &original_image,
                                            const int &w, const int &h, const int &c) const
{
    return original_image.getPixel(w, h, c);
}

template <typename WORK, typename STORE>
Image<STORE> Filter2D<WORK, STORE>::apply(const Image<STORE> &original_image,
                                          const int &arg_channels,
                                          const std::vector<int> &arg_width_range,
                                          const std::vector<int> &arg_height_range) const
{
    // optional crop parameters
    std::vector<int> new_width_range(2), new_height_range(2);
    Image<STORE>::check_range_assign(new_width_range,
                                     arg_width_range,
                                     0, original_image.getWidth() - 1);
    Image<STORE>::check_range_assign(new_height_range,
                                     arg_height_range,
                                     0, original_image.getHeight() - 1);
    int new_channels = arg_channels == -1 ? original_image.getChannels() : arg_channels;

    Image<STORE> new_image(new_width_range[1] - new_width_range[0] + 1,
                           new_height_range[1] - new_height_range[0] + 1,
                           new_channels);
    WORK temp;
    for (int i = new_height_range[0]; i <= new_height_range[1]; i++)
    {
        for (int j = new_width_range[0]; j <= new_width_range[1]; j++)
        {
            for (int k = 0; k < new_channels; k++)
            {
                // clip the pixel value to storage precision
                temp = applyPerElement(original_image, j, i, k);
                temp = std::max(0., std::min(temp, static_cast<WORK>(std::numeric_limits<STORE>::max())));
                new_image.setPixel(static_cast<STORE>(temp), j, i, k);
            }
        }
    }
    return new_image;
}

// Grayscale2D definitions
template <typename WORK, typename STORE>
Grayscale2D<WORK, STORE>::Grayscale2D(const std::vector<int> &indices,
                                      const std::vector<WORK> &coefficients)
    : channel_indices(indices), channel_coefficients(coefficients)
{
    if (channel_indices.size() != channel_coefficients.size())
    {
        throw std::invalid_argument("Indices and coefficients should be of same length");
    }
    // normalising coefficients
    WORK sum = std::accumulate(channel_coefficients.begin(),
                               channel_coefficients.end(),
                               0.);
    for (auto &c : channel_coefficients)
    {
        c /= sum;
    }
}

template <typename WORK, typename STORE>
WORK Grayscale2D<WORK, STORE>::applyPerElement(const Image<STORE> &original_image,
                                               const int &w, const int &h) const
{
    WORK output = 0.;
    for (const int &index : channel_indices)
    {
        output += original_image.getPixel(w, h, index) * channel_coefficients[index];
    }
    return static_cast<STORE>(output);
}

template <typename WORK, typename STORE>
Image<STORE> Grayscale2D<WORK, STORE>::apply(const Image<STORE> &original_image) const
{
    int max_index = *std::max_element(channel_indices.begin(), channel_indices.end());
    int min_index = *std::min_element(channel_indices.begin(), channel_indices.end());
    if ((max_index > (original_image.getChannels() - 1)) || (min_index < 0))
    {
        throw std::out_of_range("Channel indices are out of range");
    }
    return Filter2D<WORK, STORE>::apply(original_image, 1);
}

// ColourBalance2D definitions
template <typename WORK, typename STORE>
ColourBalance2D<WORK, STORE>::ColourBalance2D(const std::vector<WORK> &coefficients)
    : channel_coefficients(coefficients)
{
}

template <typename WORK, typename STORE>
WORK ColourBalance2D<WORK, STORE>::applyPerElement(const Image<STORE> &original_image,
                                                   const int &w, const int &h, const int &c) const
{
    return channel_coefficients[c] * original_image.getPixel(w, h, c);
}

template <typename WORK, typename STORE>
Image<STORE> ColourBalance2D<WORK, STORE>::apply(const Image<STORE> &original_image) const
{
    if (channel_coefficients.size() != original_image.getChannels())
    {
        throw std::invalid_argument("Colour balance ratios must exist for all channels");
    }
    return Filter2D<WORK, STORE>::apply(original_image);
}

// Brightness2D definitions
template <typename WORK, typename STORE>
Brightness2D<WORK, STORE>::Brightness2D(const WORK &arg_brightness)
    : brightness(arg_brightness)
{
}

template <typename WORK, typename STORE>
WORK Brightness2D<WORK, STORE>::applyPerElement(const Image<STORE> &original_image,
                                                const int &w, const int &h, const int &c) const
{
    return brightness + original_image.getPixel(w, h, c);
}

// Conv2D definitions
// only used by derived classes
template <typename WORK, typename STORE>
Conv2D<WORK, STORE>::Conv2D(const int &arg_kernel_size)
    : kernel_size(arg_kernel_size)
{
    if (kernel_size <= 0)
    {
        throw std::invalid_argument("Kernel must have positive size");
    }
    kernel_range.resize(2);
    if (kernel_size % 2)
    {
        kernel_range.front() = -kernel_size / 2;
    }
    else
    {
        kernel_range.front() = -kernel_size / 2 + 1;
    }
    kernel_range.back() = kernel_size / 2;
}

// Blur2D definitions
template <typename WORK, typename STORE>
Blur2D<WORK, STORE>::Blur2D(const BlurKernel &arg_kernel,
                            const int &arg_kernel_size, const WORK &stddev)
    : Conv2D<WORK, STORE>(arg_kernel_size), kernel(arg_kernel)
{
    switch (kernel)
    {
    case BlurKernel::box:
        Conv2D<WORK, STORE>::kernel_parameters.resize(Conv2D<WORK, STORE>::kernel_size * Conv2D<WORK, STORE>::kernel_size);
        std::fill(Conv2D<WORK, STORE>::kernel_parameters.begin(),
                  Conv2D<WORK, STORE>::kernel_parameters.end(),
                  1. / (Conv2D<WORK, STORE>::kernel_size * Conv2D<WORK, STORE>::kernel_size));
        break;
    case BlurKernel::gaussian:
    {
        Conv2D<WORK, STORE>::kernel_parameters.resize(Conv2D<WORK, STORE>::kernel_size * Conv2D<WORK, STORE>::kernel_size);
        int krange = Conv2D<WORK, STORE>::kernel_size / 2;
        WORK ksum = 0.;
        for (int j = 0; j < Conv2D<WORK, STORE>::kernel_size; j++)
        {
            for (int i = 0; i < Conv2D<WORK, STORE>::kernel_size; i++)
            {
                WORK denominator = 2. * M_PI * stddev * stddev;
                WORK numerator = std::exp(-((i - krange) * (i - krange) + (j - krange) * (j - krange)) / (2. * stddev * stddev));
                Conv2D<WORK, STORE>::kernel_parameters[j * Conv2D<WORK, STORE>::kernel_size + i] = numerator / denominator;
                ksum += Conv2D<WORK, STORE>::kernel_parameters[j * Conv2D<WORK, STORE>::kernel_size + i];
            }
        }
        // normalise kernel
        for (int j = 0; j < Conv2D<WORK, STORE>::kernel_size; j++)
        {
            for (int i = 0; i < Conv2D<WORK, STORE>::kernel_size; i++)
            {
                Conv2D<WORK, STORE>::kernel_parameters[j * Conv2D<WORK, STORE>::kernel_size + i] /= ksum;
            }
        }
        break;
    }
    }
}

template <typename WORK, typename STORE>
WORK Blur2D<WORK, STORE>::applyPerElement(const Image<STORE> &original_image,
                                          const int &w, const int &h, const int &c) const
{
    WORK output = 0.;
    for (int j = Conv2D<WORK, STORE>::kernel_range[0]; j <= Conv2D<WORK, STORE>::kernel_range[1]; j++)
    {
        for (int i = Conv2D<WORK, STORE>::kernel_range[0]; i <= Conv2D<WORK, STORE>::kernel_range[1]; i++)
        {
            output += Conv2D<WORK, STORE>::kernel_parameters[(j - Conv2D<WORK, STORE>::kernel_range[0]) * Conv2D<WORK, STORE>::kernel_size + (i - Conv2D<WORK, STORE>::kernel_range[0])] * original_image.getPixel(w + i, h + j, c);
        }
    }
    return output;
}

// MedianBlur2D definitions
template <typename WORK, typename STORE>
MedianBlur2D<WORK, STORE>::MedianBlur2D(const int &arg_kernel_size)
    : Conv2D<WORK, STORE>(arg_kernel_size)
{
}

template <typename WORK, typename STORE>
WORK MedianBlur2D<WORK, STORE>::applyPerElement(const Image<STORE> &original_image,
                                                const int &w, const int &h, const int &c) const
{
    std::vector<STORE> sort_pixels(Conv2D<WORK, STORE>::kernel_size * Conv2D<WORK, STORE>::kernel_size);
    for (int j = Conv2D<WORK, STORE>::kernel_range[0]; j <= Conv2D<WORK, STORE>::kernel_range[1]; j++)
    {
        for (int i = Conv2D<WORK, STORE>::kernel_range[0]; i <= Conv2D<WORK, STORE>::kernel_range[1]; i++)
        {
            sort_pixels.push_back(original_image.getPixel(w + i, h + j, c));
        }
    }
    return Quickselect<STORE>::median(sort_pixels);
}

// Edge2D definitions
template <typename WORK, typename STORE>
Edge2D<WORK, STORE>::Edge2D(const EdgeKernel &arg_kernel)
    : Conv2D<WORK, STORE>::Conv2D(3), kernel(arg_kernel)
{
    switch (kernel)
    {
    case EdgeKernel::sobel:
    {
        Conv2D<WORK, STORE>::kernel_parameters = {1., 0., -1., 2., 0., -2., 1., 0., -1.};
        break;
    }
    case EdgeKernel::prewitt:
    {
        Conv2D<WORK, STORE>::kernel_parameters = {1., 0., -1., 1., 0., -1., 1., 0., -1.};
        break;
    }
    case EdgeKernel::scharr:
    {
        Conv2D<WORK, STORE>::kernel_parameters = {3., 0., -3., 10., 0., -10., 3., 0., -3.};
        break;
    }
    case EdgeKernel::robertscross:
    {
        Conv2D<WORK, STORE>::kernel_size = 2;
        Conv2D<WORK, STORE>::kernel_range.front() = 0;
        Conv2D<WORK, STORE>::kernel_range.back() = 1;
        Conv2D<WORK, STORE>::kernel_parameters = {1., 0., 0, -1., 0., 1., -1., 0.};
        break;
    }
    }
}

template <typename WORK, typename STORE>
WORK Edge2D<WORK, STORE>::applyPerElement(const Image<STORE> &original_image,
                                          const int &w, const int &h, const int &c) const
{
    WORK output_x = 0.;
    WORK output_y = 0.;
    for (int j = Conv2D<WORK, STORE>::kernel_range[0]; j <= Conv2D<WORK, STORE>::kernel_range[1]; j++)
    {
        for (int i = Conv2D<WORK, STORE>::kernel_range[0]; i <= Conv2D<WORK, STORE>::kernel_range[1]; i++)
        {
            output_x += Conv2D<WORK, STORE>::kernel_parameters[(j - Conv2D<WORK, STORE>::kernel_range[0]) * Conv2D<WORK, STORE>::kernel_size + (i - Conv2D<WORK, STORE>::kernel_range[0])] * original_image.getPixel(w + i, h + j, c);
            switch (kernel)
            {
            case EdgeKernel::sobel:
            case EdgeKernel::prewitt:
            case EdgeKernel::scharr:
                output_y += Conv2D<WORK, STORE>::kernel_parameters[(i - Conv2D<WORK, STORE>::kernel_range[0]) * Conv2D<WORK, STORE>::kernel_size + (j - Conv2D<WORK, STORE>::kernel_range[0])] * original_image.getPixel(w + i, h + j, c);
                break;
            case EdgeKernel::robertscross:
                output_y += Conv2D<WORK, STORE>::kernel_parameters[Conv2D<WORK, STORE>::kernel_size * Conv2D<WORK, STORE>::kernel_size + j * Conv2D<WORK, STORE>::kernel_size + i] * original_image.getPixel(w + i, h + j, c);
                break;
            }
        }
    }
    return std::sqrt(output_x * output_x + output_y * output_y);
}

template <typename WORK, typename STORE>
Image<STORE> Edge2D<WORK, STORE>::apply(const Image<STORE> &original_image) const
{
    if (original_image.getChannels() != 1)
    {
        throw std::domain_error("Edge detection can only be applied on grayscale images");
    }
    else
    {
        return Filter2D<WORK, STORE>::apply(original_image);
    }
}

// HistogramEqualisation2D definitions
template <typename WORK, typename STORE>
Image<STORE> HistogramEqualisation2D<WORK, STORE>::apply(const Image<STORE> &original_image,
                                                         const int &arg_channels,
                                                         const std::vector<int> &arg_width_range,
                                                         const std::vector<int> &arg_height_range) const
{
    // optional crop parameters
    std::vector<int> new_width_range(2), new_height_range(2);
    Image<STORE>::check_range_assign(new_width_range,
                                     arg_width_range,
                                     0, original_image.getWidth() - 1);
    Image<STORE>::check_range_assign(new_height_range,
                                     arg_height_range,
                                     0, original_image.getHeight() - 1);
    int new_channels = arg_channels == -1 ? original_image.getChannels() : arg_channels;

    Image<STORE> new_image(new_width_range[1] - new_width_range[0] + 1,
                           new_height_range[1] - new_height_range[0] + 1,
                           new_channels);

    // histogram calculation
    std::vector<std::vector<std::size_t>> pdf, cdf;
    std::vector<std::size_t> cdf_min;
    for (int k = 0; k < new_channels; k++)
    {
        pdf.push_back(std::vector<std::size_t>(std::numeric_limits<STORE>::max() - std::numeric_limits<STORE>::min() + 1));
        cdf.push_back(std::vector<std::size_t>(std::numeric_limits<STORE>::max() - std::numeric_limits<STORE>::min() + 1));
    }
    for (int i = new_height_range[0]; i <= new_height_range[1]; i++)
    {
        for (int j = new_width_range[0]; j <= new_width_range[1]; j++)
        {
            for (int k = 0; k < new_channels; k++)
            {
                pdf[k][original_image.getPixel(j, i, k)]++;
            }
        }
    }
    STORE idx;
    for (int k = 0; k < new_channels; k++)
    {
        std::partial_sum(pdf[k].begin(), pdf[k].end(), cdf[k].begin());
        idx = 0;
        while (true)
        {
            if (cdf[k][idx] != 0)
            {
                cdf_min.push_back(cdf[k][idx]);
                break;
            }
            idx++;
        }
    }

    // filter application
    for (int i = new_height_range[0]; i <= new_height_range[1]; i++)
    {
        for (int j = new_width_range[0]; j <= new_width_range[1]; j++)
        {
            for (int k = 0; k < new_channels; k++)
            {
                // to guard against degenerate or singular histograms
                // especially the alpha channel
                if ((new_height_range[1] - new_height_range[0] + 1) * (new_width_range[1] - new_width_range[0] + 1) != cdf_min[k])
                {
                    new_image.setPixel(static_cast<STORE>((cdf[k][original_image.getPixel(j, i, k)] - cdf_min[k]) * (std::numeric_limits<STORE>::max() - std::numeric_limits<STORE>::min()) / ((new_height_range[1] - new_height_range[0] + 1) * (new_width_range[1] - new_width_range[0] + 1) - cdf_min[k])), j, i, k);
                }
                else
                {
                    new_image.setPixel(original_image.getPixel(j, i, k), j, i, k);
                }
            }
        }
    }
    return new_image;
}

// Filter3D definitions
// identity transform as base class
template <typename WORK, typename STORE>
WORK Filter3D<WORK, STORE>::applyPerElement(const Volume<STORE> &original_volume,
                                            const int &d, const int &w, const int &h, const int &c) const
{
    return original_volume.getVoxel(d, w, h, c);
}

template <typename WORK, typename STORE>
Volume<STORE> Filter3D<WORK, STORE>::apply(const Volume<STORE> &original_volume,
                                           const int &arg_channels,
                                           const std::vector<int> &arg_depth_range,
                                           const std::vector<int> &arg_width_range,
                                           const std::vector<int> &arg_height_range) const
{
    // optional crop parameters
    std::vector<int> new_depth_range(2), new_width_range(2), new_height_range(2);
    Image<STORE>::check_range_assign(new_depth_range,
                                     arg_depth_range,
                                     0, original_volume.getDepth() - 1);
    Image<STORE>::check_range_assign(new_width_range,
                                     arg_width_range,
                                     0, original_volume.getWidth() - 1);
    Image<STORE>::check_range_assign(new_height_range,
                                     arg_height_range,
                                     0, original_volume.getHeight() - 1);
    int new_channels = arg_channels == -1 ? original_volume.getChannels() : arg_channels;

    Volume<STORE> new_volume(new_depth_range[1] - new_depth_range[0] + 1,
                             new_width_range[1] - new_width_range[0] + 1,
                             new_height_range[1] - new_height_range[0] + 1,
                             new_channels);
    std::vector<int> temp_slice = original_volume.getIndices();
    new_volume.setIndices(std::vector<int>(temp_slice.begin() + new_depth_range[0],
                                           temp_slice.begin() + new_depth_range[1] + 1));
    WORK temp;
    for (int l = new_depth_range[0]; l <= new_depth_range[1]; l++)
    {
        for (int i = new_height_range[0]; i <= new_height_range[1]; i++)
        {
            for (int j = new_width_range[0]; j <= new_width_range[1]; j++)
            {
                for (int k = 0; k < new_channels; k++)
                {
                    // clip the pixel value to storage precision
                    temp = applyPerElement(original_volume, l, j, i, k);
                    temp = std::max(0., std::min(temp, static_cast<WORK>(std::numeric_limits<STORE>::max())));
                    new_volume.setVoxel(static_cast<STORE>(temp), l, j, i, k);
                }
            }
        }
    }
    return new_volume;
}

// Conv3D definitions
// only used by derived classes
template <typename WORK, typename STORE>
Conv3D<WORK, STORE>::Conv3D(const int &arg_kernel_size)
    : kernel_size(arg_kernel_size)
{
    if (kernel_size <= 0)
    {
        throw std::invalid_argument("Kernel must have positive size");
    }
    kernel_range.resize(2);
    if (kernel_size % 2)
    {
        kernel_range.front() = -kernel_size / 2;
    }
    else
    {
        kernel_range.front() = -kernel_size / 2 + 1;
    }
    kernel_range.back() = kernel_size / 2;
}

// Blur2D definitions
template <typename WORK, typename STORE>
Blur3D<WORK, STORE>::Blur3D(const BlurKernel &arg_kernel,
                            const int &arg_kernel_size, const WORK &stddev)
    : Conv3D<WORK, STORE>(arg_kernel_size), kernel(arg_kernel)
{
    switch (kernel)
    {
    case BlurKernel::box:
        Conv3D<WORK, STORE>::kernel_parameters.resize(Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size);
        std::fill(Conv3D<WORK, STORE>::kernel_parameters.begin(),
                  Conv3D<WORK, STORE>::kernel_parameters.end(),
                  1. / (Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size));
        break;
    case BlurKernel::gaussian:
    {
        Conv3D<WORK, STORE>::kernel_parameters.resize(Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size);
        int krange = Conv3D<WORK, STORE>::kernel_size / 2;
        WORK ksum = 0.;
        for (int k = 0; k < Conv3D<WORK, STORE>::kernel_size; k++)
        {
            for (int j = 0; j < Conv3D<WORK, STORE>::kernel_size; j++)
            {
                for (int i = 0; i < Conv3D<WORK, STORE>::kernel_size; i++)
                {
                    WORK denominator = std::pow(2. * M_PI * stddev * stddev, 1.5);
                    WORK numerator = std::exp(-((i - krange) * (i - krange) + (j - krange) * (j - krange) + (k - krange) * (k - krange)) / (2. * stddev * stddev));
                    Conv3D<WORK, STORE>::kernel_parameters[k * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size + j * Conv3D<WORK, STORE>::kernel_size + i] = numerator / denominator;
                    ksum += Conv3D<WORK, STORE>::kernel_parameters[k * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size + j * Conv3D<WORK, STORE>::kernel_size + i];
                }
            }
        }
        // normalise kernel
        for (int k = 0; k < Conv3D<WORK, STORE>::kernel_size; k++)
        {
            for (int j = 0; j < Conv3D<WORK, STORE>::kernel_size; j++)
            {
                for (int i = 0; i < Conv3D<WORK, STORE>::kernel_size; i++)
                {
                    Conv3D<WORK, STORE>::kernel_parameters[k * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size + j * Conv3D<WORK, STORE>::kernel_size + i] /= ksum;
                }
            }
        }
        break;
    }
    }
}

template <typename WORK, typename STORE>
WORK Blur3D<WORK, STORE>::applyPerElement(const Volume<STORE> &original_volume,
                                          const int &d, const int &w, const int &h, const int &c) const
{
    WORK output = 0.;
    for (int k = Conv3D<WORK, STORE>::kernel_range[0]; k <= Conv3D<WORK, STORE>::kernel_range[1]; k++)
    {
        for (int j = Conv3D<WORK, STORE>::kernel_range[0]; j <= Conv3D<WORK, STORE>::kernel_range[1]; j++)
        {
            for (int i = Conv3D<WORK, STORE>::kernel_range[0]; i <= Conv3D<WORK, STORE>::kernel_range[1]; i++)
            {
                output += Conv3D<WORK, STORE>::kernel_parameters[(k - Conv3D<WORK, STORE>::kernel_range[0]) * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size + (j - Conv3D<WORK, STORE>::kernel_range[0]) * Conv3D<WORK, STORE>::kernel_size + (i - Conv3D<WORK, STORE>::kernel_range[0])] * original_volume.getVoxel(d + k, w + i, h + j, c);
            }
        }
    }
    return output;
}

// MedianBlur3D definitions
template <typename WORK, typename STORE>
MedianBlur3D<WORK, STORE>::MedianBlur3D(const int &arg_kernel_size)
    : Conv3D<WORK, STORE>(arg_kernel_size)
{
}

template <typename WORK, typename STORE>
WORK MedianBlur3D<WORK, STORE>::applyPerElement(const Volume<STORE> &original_volume,
                                                const int &d, const int &w, const int &h, const int &c) const
{
    std::vector<STORE> sort_pixels(Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size * Conv3D<WORK, STORE>::kernel_size);
    for (int k = Conv3D<WORK, STORE>::kernel_range[0]; k <= Conv3D<WORK, STORE>::kernel_range[1]; k++)
    {
        for (int j = Conv3D<WORK, STORE>::kernel_range[0]; j <= Conv3D<WORK, STORE>::kernel_range[1]; j++)
        {
            for (int i = Conv3D<WORK, STORE>::kernel_range[0]; i <= Conv3D<WORK, STORE>::kernel_range[1]; i++)
            {
                sort_pixels.push_back(original_volume.getVoxel(d + k, w + i, h + j, c));
            }
        }
    }
    return Quickselect<STORE>::median(sort_pixels);
}

// template instantiation
template class Filter2D<double, unsigned char>;
template class Grayscale2D<double, unsigned char>;
template class ColourBalance2D<double, unsigned char>;
template class Brightness2D<double, unsigned char>;
template class Conv2D<double, unsigned char>;
template class Blur2D<double, unsigned char>;
template class MedianBlur2D<double, unsigned char>;
template class Edge2D<double, unsigned char>;
template class HistogramEqualisation2D<double, unsigned char>;

template class Filter3D<double, unsigned char>;
template class Conv3D<double, unsigned char>;
template class Blur3D<double, unsigned char>;
template class MedianBlur3D<double, unsigned char>;