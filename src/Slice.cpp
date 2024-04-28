//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

#include "Slice.hpp"
#include "Image.hpp"
#include "Volume.hpp"
#include "Projection.hpp"

#include <vector>

template <typename WORK, typename STORE>
Image<STORE> Slice<WORK, STORE>::take(const Volume<STORE> &original_volume,
                                      const Axis &slice_axis,
                                      const int &slice_location)
{
    Projection<WORK, STORE> slicing_projection;
    return slicing_projection.take(original_volume,
                                   Intensity::max,
                                   slice_axis,
                                   std::vector<int>{slice_location, slice_location});
}

// template instantiation
template class Slice<double, unsigned char>;