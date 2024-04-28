//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu#pragma once

#include "Image.hpp"
#include "Volume.hpp"
#include <vector>

enum class BlurKernel
{
    box,
    gaussian
};

enum class EdgeKernel
{
    sobel,
    prewitt,
    scharr,
    robertscross
};

// working and storage precision data types
// have been templated for all filters
template <typename WORK, typename STORE>
class Filter2D
{
public:
    virtual WORK applyPerElement(const Image<STORE> &,
                                 const int &, const int &, const int &) const;
    virtual Image<STORE> apply(const Image<STORE> &,
                               const int & = -1,
                               const std::vector<int> & = {-1, -1},
                               const std::vector<int> & = {-1, -1}) const;
};

template <typename WORK, typename STORE>
class Grayscale2D
    : public Filter2D<WORK, STORE>
{
protected:
    std::vector<int> channel_indices;
    std::vector<WORK> channel_coefficients;

public:
    Grayscale2D(const std::vector<int> & = {0, 1, 2},
                const std::vector<WORK> & = {0.2126, 0.7152, 0.0722});
    WORK applyPerElement(const Image<STORE> &,
                         const int &, const int &) const;
    Image<STORE> apply(const Image<STORE> &) const;
};

template <typename WORK, typename STORE>
class ColourBalance2D
    : public Filter2D<WORK, STORE>
{
protected:
    std::vector<WORK> channel_coefficients;

public:
    ColourBalance2D(const std::vector<WORK> &);
    WORK applyPerElement(const Image<STORE> &,
                         const int &, const int &, const int &) const;
    Image<STORE> apply(const Image<STORE> &) const;
};

template <typename WORK, typename STORE>
class Brightness2D
    : public Filter2D<WORK, STORE>
{
protected:
    WORK brightness;

public:
    Brightness2D(const WORK &);
    WORK applyPerElement(const Image<STORE> &,
                         const int &, const int &, const int &) const;
};

template <typename WORK, typename STORE>
class Conv2D
    : public Filter2D<WORK, STORE>
{
protected:
    int kernel_size;
    std::vector<int> kernel_range;
    std::vector<WORK> kernel_parameters;

public:
    Conv2D(const int &);
};

template <typename WORK, typename STORE>
class Blur2D
    : public Conv2D<WORK, STORE>
{
protected:
    BlurKernel kernel;

public:
    Blur2D(const BlurKernel &,
           const int & = 3, const WORK & = 1.);
    WORK applyPerElement(const Image<STORE> &,
                         const int &, const int &, const int &) const;
};

template <typename WORK, typename STORE>
class MedianBlur2D
    : public Conv2D<WORK, STORE>
{
public:
    MedianBlur2D(const int & = 3);
    WORK applyPerElement(const Image<STORE> &,
                         const int &, const int &, const int &) const;
};

template <typename WORK, typename STORE>
class Edge2D
    : public Conv2D<WORK, STORE>
{
protected:
    EdgeKernel kernel;

public:
    Edge2D(const EdgeKernel &);
    WORK applyPerElement(const Image<STORE> &,
                         const int &, const int &, const int &) const;
    Image<STORE> apply(const Image<STORE> &) const;
};

template <typename WORK, typename STORE>
class HistogramEqualisation2D
    : public Filter2D<WORK, STORE>
{
public:
    Image<STORE> apply(const Image<STORE> &,
                       const int & = -1,
                       const std::vector<int> & = {-1, -1},
                       const std::vector<int> & = {-1, -1}) const;
};

template <typename WORK, typename STORE>
class Filter3D
{
public:
    virtual WORK applyPerElement(const Volume<STORE> &,
                                 const int &, const int &, const int &, const int &) const;
    virtual Volume<STORE> apply(const Volume<STORE> &,
                                const int & = -1,
                                const std::vector<int> & = {-1, -1},
                                const std::vector<int> & = {-1, -1},
                                const std::vector<int> & = {-1, -1}) const;
};

template <typename WORK, typename STORE>
class Conv3D
    : public Filter3D<WORK, STORE>
{
protected:
    int kernel_size;
    std::vector<int> kernel_range;
    std::vector<WORK> kernel_parameters;

public:
    Conv3D(const int &);
};

template <typename WORK, typename STORE>
class Blur3D
    : public Conv3D<WORK, STORE>
{
protected:
    BlurKernel kernel;

public:
    Blur3D(const BlurKernel &,
           const int & = 3, const WORK & = 1.);
    WORK applyPerElement(const Volume<STORE> &,
                         const int &, const int &, const int &, const int &) const;
};

template <typename WORK, typename STORE>
class MedianBlur3D
    : public Conv3D<WORK, STORE>
{
public:
    MedianBlur3D(const int & = 3);
    WORK applyPerElement(const Volume<STORE> &,
                         const int &, const int &, const int &, const int &) const;
};