//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

#include <iostream>
#include <vector>
#include "Image.hpp"
#include "Volume.hpp"
#include "Filter.hpp"
#include "Projection.hpp"
#include "Slice.hpp"
#include <vector>
#include <chrono>
#include <fstream>

struct Test_time{
  double min, max, avg;
};

Test_time get_test_time(const std::vector<double>& times){
  Test_time t;
  t.min = std::numeric_limits<double>::max();
  t.max = t.avg = 0;
  for(size_t i = 0; i < times.size(); i++){
    t.min = std::min(t.min, times[i]);
    t.max = std::max(t.max, times[i]);
    t.avg += times[i];
  }
  t.avg /= times.size();
  return t;
}

// this function tests all the filters and write out put in a txt file
void test_performance_2D(){
  // a vector to store test name and time comsuming
  std::vector<std::string> total_test_name;
  std::vector<Test_time> total_test_time;

  // initializations
  std::vector<double> single_test_time;
  int test_count = 10;
  std::vector<Image<unsigned char>*> test_images;

  // test grayscale
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }

  total_test_name.push_back("grayscale");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test colorbalance
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{255/210., 255/214., 255/249., 1});
    auto output_image = colourbalance.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("colorbalance");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test brightness
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Brightness2D<double, unsigned char> brightness(-100);
    auto output_image = brightness.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("brightness");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  std::cout << "test median-blur-k3" << std::endl;
  // test median-blur-k3
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    MedianBlur2D<double, unsigned char> medianBlur2D(3);
    auto output_image = medianBlur2D.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("median-blur-k3");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();


  std::cout << "test median-blur-k5" << std::endl;
  // test median-blur-k5
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    MedianBlur2D<double, unsigned char> medianBlur2D(5);
    auto output_image = medianBlur2D.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("median-blur-k5");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  std::cout << "test median-blur-k7" << std::endl;
  // test median-blur-k7
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    MedianBlur2D<double, unsigned char> medianBlur2D(7);
    auto output_image = medianBlur2D.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("median-blur-k7");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  std::cout << "test median-blur-k9" << std::endl;
  // test median-blur-k9
  for (int i = 0; i < test_count; i++){
    std::cout << "test median-blur-k9:"<< i << std::endl;
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    MedianBlur2D<double, unsigned char> medianBlur2D(9);
    auto output_image = medianBlur2D.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("median-blur-k9");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test box-blur-k3
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::box,3);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("box-blur-k3");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test box-blur-k5
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::box,5);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("box-blur-k5");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test box-blur-k7
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::box,7);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("box-blur-k7");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test box-blur-k9
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::box,9);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("box-blur-k9");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test gaussian-blur-k3
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::gaussian,3);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian-box-k3");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test gaussian-blur-k5
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::gaussian,5);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian-box-k5");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test gaussian-blur-k7
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::gaussian,7);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian-box-k7");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test gaussian-blur-k9
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Blur2D<double, unsigned char> blur(BlurKernel::gaussian,9);
    auto output_image = blur.apply(test_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian-box-k9");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // edge detection edge_acharr
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::scharr);
    auto output_image = edge.apply(intermediate_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("edge_acharr");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // edge detection edge_prewitt
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::prewitt);
    auto output_image = edge.apply(intermediate_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("edge_prewitt");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // edge detection edge_robertscross
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::robertscross);
    auto output_image = edge.apply(intermediate_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("edge_robertscross");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // edge detection edge_sobel
  for (int i = 0; i < test_count; i++){
    auto test_image = Image<unsigned char>("../Images/gracehopper.png");
    auto start_time = std::chrono::high_resolution_clock::now();
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::sobel);
    auto output_image = edge.apply(intermediate_image);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("edge_sobel");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // show results
  for (int i = 0; i < total_test_name.size(); i++){
    std::cout << total_test_name[i] << " " << total_test_time[i].min << " " << total_test_time[i].max << " " << total_test_time[i].avg << std::endl;
  }

  // Open an output file stream
  std::ofstream outfile("performance_2D.txt");

  // Check if the file is open
  if (!outfile.is_open()) {
    std::cerr << "Failed to open the output file" << std::endl;
  }

  // Write the vector of strings to the file
  for (int i = 0; i < total_test_name.size(); i++){
    outfile << total_test_name[i] << " " << total_test_time[i].min << " " << total_test_time[i].max << " " << total_test_time[i].avg << std::endl;
  }

  // Close the output file stream
  outfile.close();
}

void test_performance_3D() {
  // a vector to store test name and time comsuming
  std::vector<std::string> total_test_name;
  std::vector<Test_time> total_test_time;

  // initializations
  std::vector<double> single_test_time;
  int test_count = 3;
  std::vector<Image<unsigned char>*> test_images;

  // test confuYZ with 20 images
  // test MinIP
  for (int i = 0; i < test_count; i++){
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Projection<double, unsigned char> projection;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_image = projection.take(test_volume, Intensity::min, Axis::elevation);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }

  total_test_name.push_back("MinIP");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test MIP
  for (int i = 0; i < test_count; i++){
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Projection<double, unsigned char> projection;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_image = projection.take(test_volume, Intensity::max, Axis::elevation);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }

  total_test_name.push_back("MIP");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test AIP_mean
  for (int i = 0; i < test_count; i++){
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Projection<double, unsigned char> projection;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_image = projection.take(test_volume, Intensity::mean, Axis::elevation);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }

  total_test_name.push_back("AIP_mean");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test AIP_median
  for (int i = 0; i < test_count; i++){
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Projection<double, unsigned char> projection;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_image = projection.take(test_volume, Intensity::median, Axis::elevation);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }

  total_test_name.push_back("AIP_median");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();


  // test gaussian_k3
  for (int i = 0; i < test_count; i++){
    std::cout << "test gaussian_k3" << i <<std::endl;
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Blur3D<double, unsigned char> blur(BlurKernel::gaussian, 3);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian_k3");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test gaussian_k5
  for (int i = 0; i < test_count; i++){
    std::cout << "test gaussian_k5" << i <<std::endl;
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Blur3D<double, unsigned char> blur(BlurKernel::gaussian, 5);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian_k5");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test gaussian_k7
  for (int i = 0; i < test_count; i++){
    std::cout << "test gaussian_k7" << i <<std::endl;
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Blur3D<double, unsigned char> blur(BlurKernel::gaussian, 7);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian_k7");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test gaussian_k9
  for (int i = 0; i < test_count; i++){
    std::cout << "test gaussian_k9" << i <<std::endl;
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    Blur3D<double, unsigned char> blur(BlurKernel::gaussian, 9);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("gaussian_k9");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test median_k3
  for (int i = 0; i < test_count; i++){
    std::cout << "test median_k3" << i <<std::endl;
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    MedianBlur3D<double, unsigned char> blur(3);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("median_k3");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test median_k5
  for (int i = 0; i < test_count; i++){
    std::cout << "test median_k5" << i <<std::endl;
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    MedianBlur3D<double, unsigned char> blur(5);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
    std::cout << "test median_k5" << i << " time spent: " << single_test_time[i] <<std::endl;
  }
  total_test_name.push_back("median_k5");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // test median_k7
  for (int i = 0; i < test_count; i++){
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    MedianBlur3D<double, unsigned char> blur(7);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
    std::cout << "test median_k7" << i << " time spent: " << single_test_time[i] <<std::endl;
  }
  total_test_name.push_back("median_k7");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();


  // test median_k9
  for (int i = 0; i < test_count; i++){
    auto test_volume = Volume<unsigned char>("../Scans/confuciusornis/confuYZ", 1, 20);
    MedianBlur3D<double, unsigned char> blur(9);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto output_volume = blur.apply(test_volume);
    auto end_time = std::chrono::high_resolution_clock::now();
    single_test_time.push_back(std::chrono::duration<double, std::milli>(end_time - start_time).count());
  }
  total_test_name.push_back("median_k9");
  total_test_time.push_back(get_test_time(single_test_time));
  single_test_time.clear();

  // show results
  for (int i = 0; i < total_test_name.size(); i++){
    std::cout << total_test_name[i] << " " << total_test_time[i].min << " " << total_test_time[i].max << " " << total_test_time[i].avg << std::endl;
  }

  // Open an output file stream
  std::ofstream outfile("performance_3D_foncu_20images.txt");

  // Check if the file is open
  if (!outfile.is_open()) {
    std::cerr << "Failed to open the output file" << std::endl;
  }

  // Write the vector of strings to the file
  for (int i = 0; i < total_test_name.size(); i++){
    outfile << total_test_name[i] << " " << total_test_time[i].min << " " << total_test_time[i].max << " " << total_test_time[i].avg << std::endl;
  }

  // Close the output file stream
  outfile.close();
}

void test_filter_2D(){
  //call function to test example image and store them.
  // change the definition if you want to check other images.
  std::string file_name= "../Images/gracehopper.png";
//  std::string file_name= "../Images/tienshan.png";
//  std::string file_name= "../Images/vh_anatomy.png";
//  std::string file_name= "../Images/dimorphos.png";
//  std::string file_name= "../Images/stinkbug.png";
//  std::string file_name= "../Images/vh_ct.png";

  // test grayscale: do not do grayscale if image is 8-bits
//  {
//    auto test_image = Image<unsigned char>(file_name.c_str());
//    Grayscale2D<double, unsigned char> grayscale;
//    auto output_image = grayscale.apply(test_image);
//    output_image.write("grayscale.png");
//  }

  // test colorbalance
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    // in out colorbalance, user shall input a value of pixel as white colour
    // for gracehoper
    ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{255./254, 255./250, 255./255.,1});
//    // for tienshan
//    ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{255./252, 255./250, 255./235.,1});
//    // for vh_anatomy
//    ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{255./244, 255./244, 255./238.});
//    // for dimorphos
//    ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{255./255.});
//    // for stinkbug
//    ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{255./217.});
//    // for vh_ct
//    ColourBalance2D<double, unsigned char> colourbalance(std::vector<double>{255./170.});
    auto output_image = colourbalance.apply(test_image);
    output_image.write("colorbalance.png");
  }

  // test brightness
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Brightness2D<double, unsigned char> brightness(-100);
    auto output_image = brightness.apply(test_image);
    output_image.write("brightness_minus100.png");
  }

  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Brightness2D<double, unsigned char> brightness(100);
    auto output_image = brightness.apply(test_image);
    output_image.write("brightness_plus100.png");
  }

  // test median-blur
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    MedianBlur2D<double, unsigned char> medianBlur2D(3);
    auto output_image = medianBlur2D.apply(test_image);
    output_image.write("medianBlur2D_k3.png");
  }

  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    MedianBlur2D<double, unsigned char> medianBlur2D(5);
    auto output_image = medianBlur2D.apply(test_image);
    output_image.write("medianBlur2D_k5.png");
  }

  // test box-blur-k3
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Blur2D<double, unsigned char> blur(BlurKernel::box,3);
    auto output_image = blur.apply(test_image);
    output_image.write("boxBlur2D_k3.png");
  }

  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Blur2D<double, unsigned char> blur(BlurKernel::box,5);
    auto output_image = blur.apply(test_image);
    output_image.write("boxBlur2D_k5.png");
  }

  // test gaussian-blur-k3
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Blur2D<double, unsigned char> blur(BlurKernel::gaussian,3);
    auto output_image = blur.apply(test_image);
    output_image.write("gaussianBlur2D_k3.png");
  }

  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Blur2D<double, unsigned char> blur(BlurKernel::gaussian,5);
    auto output_image = blur.apply(test_image);
    output_image.write("gaussianBlur2D_k5.png");
  }

  // edge detection edge_acharr
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::scharr);
    auto output_image = edge.apply(intermediate_image);
    output_image.write("edge_acharr.png");
  }

  // edge detection edge_prewitt
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::prewitt);
    auto output_image = edge.apply(intermediate_image);
    output_image.write("edge_prewitt.png");
  }

  // edge detection edge_robertscross
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::robertscross);
    auto output_image = edge.apply(intermediate_image);
    output_image.write("edge_robertscross.png");
  }

  // edge detection edge_sobel
  {
    auto test_image = Image<unsigned char>(file_name.c_str());
    Grayscale2D<double, unsigned char> grayscale;
    auto intermediate_image = grayscale.apply(test_image);
    Edge2D<double, unsigned char> edge(EdgeKernel::sobel);
    auto output_image = edge.apply(intermediate_image);
    output_image.write("edge_sobel.png");
  }
}

int main(int argc, char **argv)
{
//    // call function to test performance
//    test_performance_2D();
//    // test 3D performance
//    test_performance_3D();

    // call function to test example image and store them.

    // test grayscale
    test_filter_2D();


    return 0;
}