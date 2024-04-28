//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

#include "Quickselect.hpp"

#include <algorithm>
#include <vector>

template <typename T>
std::size_t Quickselect<T>::partition(std::vector<T> &vec,
                                      const std::size_t &begin, const std::size_t &end)
{
    T pivot = vec[end];
    std::size_t i = begin - 1;
    for (std::size_t j = begin; j < end; j++)
    {
        if (vec[j] < pivot)
        {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[end]);
    return i + 1;
}

template <typename T>
T Quickselect<T>::quickselect(std::vector<T> &vec,
                              const std::size_t &begin, const std::size_t &end, const std::size_t &index)
{
    if (begin == end)
    {
        return vec[begin];
    }
    std::size_t pivot_index = partition(vec, begin, end);
    if (index == pivot_index)
    {
        return vec[index];
    }
    else if (index < pivot_index)
    {
        return quickselect(vec, begin, pivot_index - 1, index);
    }
    else
    {
        return quickselect(vec, pivot_index + 1, end, index);
    }
}

template <typename T>
T Quickselect<T>::median(std::vector<T> &vec)
{
    if (vec.size() % 2)
    {
        return quickselect(vec, 0, vec.size() - 1, (vec.size() - 1) / 2);
    }
    else
    {
        return (quickselect(vec, 0, vec.size() - 1, vec.size() / 2 - 1) + quickselect(vec, 0, vec.size() - 1, vec.size() / 2)) / 2;
    }
}

// template instantiation
template class Quickselect<unsigned char>;