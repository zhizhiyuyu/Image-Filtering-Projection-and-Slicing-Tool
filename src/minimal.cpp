#include <iostream>
#include <vector>
#include "Image.hpp"
#include "Volume.hpp"
#include "Filter.hpp"
#include "Projection.hpp"
#include "Slice.hpp"

int old_main(int argc, char **argv)
{
    auto test_image = Image<unsigned char>("../Images/tienshan.png");
    // auto test_image = Image<unsigned char>("../Scans/fracture/granite1_0325.png");

    // ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{2., 1., 1., 1.});
    // auto output_image = colourbalance.apply(test_image);
    // Brightness2D<double, unsigned char> brightness(-100);
    // auto output_image = brightness.apply(test_image);
    // Blur2D<double, unsigned char> blur(BlurKernel::box);
    // MedianBlur2D<double, unsigned char> blur;
    // auto output_image = blur.apply(test_image);
    HistogramEqualisation2D<double, unsigned char> histeq;
    auto output_image = histeq.apply(test_image);

    // Grayscale2D<double, unsigned char> grayscale;
    // auto intermediate_image = grayscale.apply(test_image);
    // Edge2D<double, unsigned char> edge(EdgeKernel::scharr);
    // auto output_image = edge.apply(intermediate_image);
    output_image.write("../Output/test_output.png");

    // auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 265);
    // auto test_volume = Volume<unsigned char>("../Scans/fracture/granite1_", 325, 1624);
    // Projection<double, unsigned char> projection;
    // auto output_image = projection.take(test_volume, Intensity::mean, Axis::elevation);
    // Slice<double, unsigned char> slice;
    // auto output_image = slice.take(test_volume, Axis::plan, 420);
    // auto output_image = slice.take(test_volume, Axis::sideview, 400);
    // auto output_image = slice.take(test_volume, Axis::plan, 138);
    // auto output_image = slice.take(test_volume, Axis::sideview, 275);
    // output_image.write("../Output/test_output.png");
    // Blur3D<double, unsigned char> blur(BlurKernel::box);
    // MedianBlur3D<double, unsigned char> blur;
    // auto output_volume = blur.apply(test_volume);
    // output_volume.write("../Output/confuciusornis/confuYZ");
    // output_volume.write("../Output/fracture/granite1_");

    return 0;
}