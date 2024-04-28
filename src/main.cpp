#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include "Image.hpp"
#include "Volume.hpp"
#include "Filter.hpp"
#include "Projection.hpp"
#include "Slice.hpp"

std::vector<std::string> tokenise(const std::string &fullstring,
                                  const char separator)
{
    std::stringstream stringcopy(fullstring);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(stringcopy, token, separator))
    {
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

int main(int argc, char **argv)
{
    std::fstream options_file;
    std::string options_string;
    std::vector<std::string> options_tokens;
    std::vector<int> options_integers;

    Image<unsigned char> last_image(1, 1, 1);
    std::vector<Image<unsigned char> *> image_vector;
    std::vector<Volume<unsigned char> *> volume_vector;

    // read in command line argument
    if (argc > 1)
    {
        options_file.open(argv[1], std::fstream::in);
    }
    else
    {
        options_file.open("../options.txt", std::fstream::in);
    }

    // check is file is accessible
    if (options_file.is_open())
    {
        while (options_file.good())
        {
            std::getline(options_file, options_string);
            // filter out comments
            if (options_string.starts_with("#"))
            {
                continue;
            }
            else
            {
                try
                {
                    // parse into instruction and operands
                    options_tokens = tokenise(options_string, ' ');
                    if (options_tokens[0] == "ImageIn:")
                    {
                        image_vector.push_back(new Image<unsigned char>(options_tokens[1].c_str()));
                    }
                    else if (options_tokens[0] == "VolumeIn:")
                    {
                        if (options_tokens.size() == 4)
                        {
                            volume_vector.push_back(new Volume<unsigned char>(options_tokens[1].c_str(),
                                                                              std::stoi(options_tokens[2]),
                                                                              std::stoi(options_tokens[3])));
                        }
                        else if (options_tokens.size() == 5)
                        {
                            volume_vector.push_back(new Volume<unsigned char>(options_tokens[1].c_str(),
                                                                              std::stoi(options_tokens[2]),
                                                                              std::stoi(options_tokens[3]),
                                                                              std::stoi(options_tokens[4])));
                        }
                        else if (options_tokens.size() == 6)
                        {
                            volume_vector.push_back(new Volume<unsigned char>(options_tokens[1].c_str(),
                                                                              std::stoi(options_tokens[2]),
                                                                              std::stoi(options_tokens[3]),
                                                                              std::stoi(options_tokens[4]),
                                                                              options_tokens[5].c_str()));
                        }
                        else
                        {
                            throw std::invalid_argument("Wrong number of arguments for loading a volume");
                        }
                    }
                    else if (options_tokens[0] == "Filter:")
                    {
                        if (options_tokens[1] == "Grayscale")
                        {
                            Grayscale2D<double, unsigned char> gs;
                            last_image = gs.apply(*(image_vector.back()));
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "ColourBalance")
                        {
                            if (options_tokens.size() == 5)
                            {
                                ColourBalance2D<double, unsigned char> cb(std::vector<double>{std::stod(options_tokens[2]),
                                                                                              std::stod(options_tokens[3]),
                                                                                              std::stod(options_tokens[4]),
                                                                                              1.});
                                last_image = cb.apply(*(image_vector.back()));
                                image_vector.push_back(&last_image);
                            }
                        }
                        else if (options_tokens[1] == "Brightness")
                        {
                            if (options_tokens.size() == 3)
                            {
                                Brightness2D<double, unsigned char> br(static_cast<unsigned char>(std::stoi(options_tokens[2])));
                                last_image = br.apply(*(image_vector.back()));
                                image_vector.push_back(&last_image);
                            }
                        }
                        else if (options_tokens[1] == "HistEq")
                        {
                            HistogramEqualisation2D<double, unsigned char> he;
                            last_image = he.apply(*(image_vector.back()));
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "MedianBlur2D")
                        {
                            if (options_tokens.size() == 2)
                            {
                                MedianBlur2D<double, unsigned char> median;
                                last_image = median.apply(*(image_vector.back()));
                            }
                            else if (options_tokens.size() == 3)
                            {
                                MedianBlur2D<double, unsigned char> median(std::stoi(options_tokens[2]));
                                last_image = median.apply(*(image_vector.back()));
                            }
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "BoxBlur2D")
                        {
                            if (options_tokens.size() == 2)
                            {
                                Blur2D<double, unsigned char> box(BlurKernel::box);
                                last_image = box.apply(*(image_vector.back()));
                            }
                            else if (options_tokens.size() == 3)
                            {
                                Blur2D<double, unsigned char> box(BlurKernel::box,
                                                                  std::stoi(options_tokens[2]));
                                last_image = box.apply(*(image_vector.back()));
                            }
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "GaussianBlur2D")
                        {
                            if (options_tokens.size() == 2)
                            {
                                Blur2D<double, unsigned char> gauss(BlurKernel::gaussian);
                                last_image = gauss.apply(*(image_vector.back()));
                            }
                            else if (options_tokens.size() == 3)
                            {
                                Blur2D<double, unsigned char> gauss(BlurKernel::gaussian,
                                                                    std::stoi(options_tokens[2]));
                                last_image = gauss.apply(*(image_vector.back()));
                            }
                            else if (options_tokens.size() == 4)
                            {
                                Blur2D<double, unsigned char> gauss(BlurKernel::gaussian,
                                                                    std::stoi(options_tokens[2]),
                                                                    std::stod(options_tokens[3]));
                                last_image = gauss.apply(*(image_vector.back()));
                            }
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "GaussianBlur2D")
                        {
                            if (options_tokens.size() == 2)
                            {
                                Blur2D<double, unsigned char> gauss(BlurKernel::gaussian);
                                last_image = gauss.apply(*(image_vector.back()));
                            }
                            else if (options_tokens.size() == 3)
                            {
                                Blur2D<double, unsigned char> gauss(BlurKernel::gaussian,
                                                                    std::stoi(options_tokens[2]));
                                last_image = gauss.apply(*(image_vector.back()));
                            }
                            else if (options_tokens.size() == 4)
                            {
                                Blur2D<double, unsigned char> gauss(BlurKernel::gaussian,
                                                                    std::stoi(options_tokens[2]),
                                                                    std::stod(options_tokens[3]));
                                last_image = gauss.apply(*(image_vector.back()));
                            }
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "Sobel")
                        {
                            Edge2D<double, unsigned char> sobel(EdgeKernel::sobel);
                            last_image = sobel.apply(*(image_vector.back()));
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "Prewitt")
                        {
                            Edge2D<double, unsigned char> prewitt(EdgeKernel::prewitt);
                            last_image = prewitt.apply(*(image_vector.back()));
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "Scharr")
                        {
                            Edge2D<double, unsigned char> scharr(EdgeKernel::scharr);
                            last_image = scharr.apply(*(image_vector.back()));
                            image_vector.push_back(&last_image);
                        }
                        else if (options_tokens[1] == "RobertsCross")
                        {
                            Edge2D<double, unsigned char> robertscross(EdgeKernel::robertscross);
                            last_image = robertscross.apply(*(image_vector.back()));
                            image_vector.push_back(&last_image);
                        }
                    }
                    else if (options_tokens[0] == "Projection:")
                    {
                    }
                    else if (options_tokens[0] == "Slice:")
                    {
                    }
                    else if (options_tokens[0] == "ImageOut:")
                    {
                        image_vector.back()->write(options_tokens[1].c_str());
                    }
                    else if (options_tokens[0] == "VolumeOut:")
                    {
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
        // free heap
        // for (const auto &image : image_vector)
        // {
        //     delete image;
        // }
        for (const auto &volume : volume_vector)
        {
            delete volume;
        }
        options_file.close();
    }
    else
    {
        std::cerr << "Options file cannot be accessed" << std::endl;
    }
    return 0;
}