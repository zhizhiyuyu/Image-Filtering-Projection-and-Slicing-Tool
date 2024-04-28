//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

#include "Projection.hpp"
#include "Image.hpp"
#include "Volume.hpp"
#include "Quickselect.hpp"
#include <algorithm>
#include <limits>
#include <vector>

template <typename WORK, typename STORE>
Image<STORE> Projection<WORK, STORE>::take(const Volume<STORE> &original_volume,
                                           const Intensity &projection_intensity,
                                           const Axis &projection_axis,
                                           const std::vector<int> &slice_limits)
{
    // permuting iteration axes
    std::vector<int> dimensions(3);
    int image_channels = original_volume.getChannels();
    switch (projection_axis)
    {
    case Axis::elevation:
        dimensions[0] = original_volume.getDepth();
        dimensions[1] = original_volume.getWidth();
        dimensions[2] = original_volume.getHeight();
        break;
    case Axis::sideview:
        dimensions[0] = original_volume.getWidth();
        dimensions[1] = original_volume.getDepth();
        dimensions[2] = original_volume.getHeight();
        break;
    case Axis::plan:
        dimensions[0] = original_volume.getHeight();
        dimensions[1] = original_volume.getWidth();
        dimensions[2] = original_volume.getDepth();
        break;
    }

    // iteration limits for slicing
    std::vector<int> iteration_limits(2);
    Image<STORE>::check_range_assign(iteration_limits,
                                     slice_limits,
                                     0, dimensions[0] - 1);

    Image<STORE> projected_image(dimensions[1], dimensions[2], image_channels);
    Image<long> sum_image(dimensions[1], dimensions[2], image_channels);

    std::size_t sort_index;
    std::vector<std::vector<STORE>> sort_voxels(dimensions[1] * dimensions[2] * image_channels);

    // saturate projection pixels for minimum value
    if (projection_intensity == Intensity::min)
    {
        for (int l = 0; l < image_channels; l++)
        {
            for (int j = 0; j < dimensions[2]; j++)
            {
                for (int k = 0; k < dimensions[1]; k++)
                {
                    projected_image.setPixel(std::numeric_limits<STORE>::max(), k, j, l);
                }
            }
        }
    }

    STORE temp;
    for (int i = iteration_limits[0]; i <= iteration_limits[1]; i++)
    {
        for (int j = 0; j < dimensions[2]; j++)
        {
            for (int k = 0; k < dimensions[1]; k++)
            {
                for (int l = 0; l < image_channels; l++)
                {
                    // same permutation as above
                    switch (projection_axis)
                    {
                    case Axis::elevation:
                        temp = original_volume.getVoxel(i, k, j, l);
                        break;
                    case Axis::sideview:
                        temp = original_volume.getVoxel(k, i, j, l);
                        break;
                    case Axis::plan:
                        temp = original_volume.getVoxel(j, k, i, l);
                        break;
                    }
                    switch (projection_intensity)
                    {
                    case Intensity::min:
                        projected_image.setPixel(std::min(temp, projected_image.getPixel(k, j, l)), k, j, l);
                        break;
                    case Intensity::max:
                        projected_image.setPixel(std::max(temp, projected_image.getPixel(k, j, l)), k, j, l);
                        break;
                    case Intensity::mean:
                        sum_image.setPixel(temp + sum_image.getPixel(k, j, l), k, j, l);
                        break;
                    case Intensity::median:
                        sort_voxels[j * dimensions[1] * image_channels + k * image_channels + l].push_back(temp);
                        break;
                    }
                }
            }
        }
    }

    for (int j = 0; j < dimensions[2]; j++)
    {
        for (int k = 0; k < dimensions[1]; k++)
        {
            for (int l = 0; l < image_channels; l++)
            {
                if (projection_intensity == Intensity::mean)
                {
                    projected_image.setPixel(static_cast<STORE>(sum_image.getPixel(k, j, l) / (iteration_limits[1] - iteration_limits[0])), k, j, l);
                }
                else if (projection_intensity == Intensity::median)
                {
                    projected_image.setPixel(Quickselect<STORE>::median(sort_voxels[j * dimensions[1] * image_channels + k * image_channels + l]), k, j, l);
                }
            }
        }
    }
    return projected_image;
}

// template instantiation
template class Projection<double, unsigned char>;