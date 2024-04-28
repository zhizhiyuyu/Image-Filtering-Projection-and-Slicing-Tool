//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

/**
 * @file
 * @brief Contains a class for performing projections of volumetric data onto 2D images.
 */

#pragma once

#include "Image.hpp"
#include "Volume.hpp"
#include <vector>

/**
 * @brief Enumerates the types of intensity values to use for projection.
 */
enum class Intensity
{
    min,
    max,
    mean,
    median
};

/**
 * @brief Class for performing projections of volumetric data onto 2D images.
 * @tparam WORK The working precision data type of the projection.
 * @tparam STORE The storage precision data type of the projection.
 */
template <typename WORK, typename STORE>
class Projection
{
public:
    /**
     * @brief Computes a 2D projection of volumetric data.
     *
     * This method computes a 2D projection of the volumetric data along the specified axis, and with the specified intensity value.
     * @param[in] volume The volumetric data to project.
     * @param[in] intensity The type of intensity value to use for the projection.
     * @param[in] axis The axis along which to project the data.
     * @param[in] range The range of the projection along the specified axis.
     * @return The 2D image containing the projection.
     */
    Image<STORE> take(const Volume<STORE> &,
                      const Intensity &,
                      const Axis & = Axis::elevation,
                      const std::vector<int> & = {-1, -1});
};