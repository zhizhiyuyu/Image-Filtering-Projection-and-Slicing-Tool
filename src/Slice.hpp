//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

/**
 * @file
 * @brief Contains a class for extracting 2D slices from 3D volumetric data.
 */
#pragma once

#include "Image.hpp"
#include "Volume.hpp"
#include "Projection.hpp"

/**
 * @brief Class for extracting 2D slices from 3D volumetric data.
 * @tparam WORK The working precision data type of the slice.
 * @tparam STORE The storage precision data type of the slice.
 */
template <typename WORK, typename STORE>
class Slice
{
public:
    /**
     * @brief Extracts a 2D slice from 3D volumetric data along a specified axis.
     *
     * This method extracts a 2D slice from the 3D volumetric data along the specified axis at the specified index.
     * @param[in] volume The 3D volumetric data to extract the slice from.
     * @param[in] axis The axis along which to extract the slice.
     * @param[in] index The index of the slice along the specified axis.
     * @return The 2D image representing the extracted slice.
     */
    Image<STORE> take(const Volume<STORE> &,
                      const Axis &,
                      const int &);
};