//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

/**
 * @file
 * @brief Contains a class for representing volumetric data.
 */
#pragma once

#include "Image.hpp"
#include <vector>

/**
 * @brief Enumerates the axes along which to slice or project volumetric data.
 */
enum class Axis
{
    sideview,
    plan,
    elevation
};

/**
 * @brief Class for representing volumetric data as a collection of 2D image slices.
 * @tparam DATA The data type of the elements in the volumetric data.
 */
template <typename DATA = unsigned char>
class Volume
{
protected:
    int depth, width, height, channels;
    std::vector<int> image_indices;
    std::vector<Image<DATA> *> image_slices;

public:
    /**
     * @brief Constructor for reading thin or thick slab volumetric data from file.
     *
     * This constructor reads in volumetric data from a series of files in sequence along the specified axis. The files can either be thin or thick slabs, depending on the specified number of slices per image.
     * @param[in] filename The base filename of the volumetric data files.
     * @param[in] axis The axis along which the slices are arranged in the files.
     * @param[in] slices_per_image The number of slices per image in the files.
     * @param[in] file_extension The file extension of the volumetric data files.
     */
    Volume(const char *,
           const int &, const int &,
           const std::size_t & = 4,
           const char * = ".png");
    /**
     * @brief Constructor for reading interleaved or explicitly defined ranges of volumetric data from file.
     *
     * This constructor reads in volumetric data from a series of files with interleaved indices, or explicitly defined ranges along the specified axis.
     * @param[in] filename The base filename of the volumetric data files.
     * @param[in] ranges The ranges of the volumetric data files along the specified axis.
     * @param[in] file_extension The file extension of the volumetric data files.
     */
    Volume(const char *,
           const std::vector<int> &,
           const std::size_t & = 4,
           const char * = ".png");
    /**
     * @brief Constructor for creating a user-defined volumetric data object.
     *
     * This constructor creates a user-defined volumetric data object with the specified dimensions and number of channels.
     * @param[in] depth The number of slices in the volumetric data.
     * @param[in] width The width of each slice in the volumetric data.
     * @param[in] height The height of each slice in the volumetric data.
     * @param[in] channels The number of channels in the volumetric data.
     */
    Volume(const int &, const int &, const int &, const int &);
    ~Volume();

    const int &getDepth() const;
    const int &getWidth() const;
    const int &getHeight() const;
    const int &getChannels() const;

    const std::vector<int> &getIndices() const;
    void setIndices(const std::vector<int> &);

    void setVoxel(const DATA &,
                  const int &, const int &, const int &, const int & = 0);
    const DATA &getVoxel(const int &, const int &, const int &, const int & = 0,
                         const PaddingType & = PaddingType::extend,
                         const DATA & = 0) const;

    static std::vector<int> return_iota(const int &, const int &);

    void write(const char *,
               const std::size_t & = 4,
               const char * = ".png") const;
};