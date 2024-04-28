//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

/**
 * @file
 * @brief Contains a class for representing and manipulating images.
 */
#pragma once

#include <vector>

/**
 * @brief Enumerates the types of image allocation modes supported by this library.
 */
enum class ImageAllocation
{
    stbi_read,
    user_heap
};

/**
 * @brief Enumerates the types of padding to use for out-of-bounds pixel accesses.
 */
enum class PaddingType
{
    constant,
    extend,
    wrap,
    reflect
};

/**
 * @brief Class for representing and manipulating images.
 * @tparam DATA The data type of the image pixels.
 */
template <typename DATA = unsigned char>
class Image
{
protected:
    ImageAllocation image_allocation;
    int width, height, channels;
    DATA *pixels;

public:
    /**
     * @brief Constructs an Image object from a file.
     * @param[in] filename The filename of the image file to read.
     */
    Image(const char *);
    /**
     * @brief Constructs an Image object with user-allocated memory.
     * @param[in] width The width of the image.
     * @param[in] height The height of the image.
     * @param[in] channels The number of channels of the image.
     */
    Image(const int &, const int &, const int &);
    Image &operator=(const Image<DATA> &);
    ~Image();

    /**
     * @brief Destructor for the Image object.
     */
    const int &getWidth() const;
    const int &getHeight() const;
    const int &getChannels() const;

    /**
     * @brief Sets the pixel value at the specified position.
     * @param[in] value The value to set the pixel to.
     * @param[in] row The row index of the pixel.
     * @param[in] col The column index of the pixel.
     * @param[in] channel The channel index of the pixel.
     */
    void setPixel(const DATA &,
                  const int &, const int &, const int & = 0);
    const DATA &getPixel(const int &, const int &, const int & = 0,
                         const PaddingType & = PaddingType::extend,
                         const DATA & = 0) const;

    static const int reflect_into_domain(const int &,
                                         const int &, const int &);
    static const int wrap_into_domain(const int &,
                                      const int &, const int &);
    static void check_range_assign(std::vector<int> &,
                                   const std::vector<int> &,
                                   const int &, const int &);

    void write(const char *) const;
};