//Group name: Binary search
//Group menbers: (Ordered by First Name)
//              Debnath, Sourajit
//              Shi, Zhancheng
//              Shu, Jessica
//              Wang, Zexing
//              Zhang, Zhiyu

/**
 * @file
 * @brief Contains a class for performing quickselect and median selection algorithms.
 */

#pragma once

#include <vector>

/**
 * @brief Class for performing quickselect and median selection algorithms on a vector.
 * @tparam T The data type of the elements in the vector.
 */
template <typename T>
class Quickselect
{
public:
    /**
     * @brief Partitions a vector around a pivot element.
     *
     * This method partitions the given vector such that all elements less than the pivot element appear before it, and all elements greater than or equal to it appear after it.
     * @param[in,out] vec The vector to partition.
     * @param[in] lo The starting index of the partition.
     * @param[in] hi The ending index of the partition.
     * @return The index of the pivot element after partitioning.
     */
    static std::size_t partition(std::vector<T> &,
                                 const std::size_t &, const std::size_t &);
    /**
     * @brief Performs the quickselect algorithm on a vector.
     *
     * This method returns the k-th smallest element of the given vector after partitioning it.
     * @param[in,out] vec The vector to perform quickselect on.
     * @param[in] lo The starting index of the quickselect.
     * @param[in] hi The ending index of the quickselect.
     * @param[in] k The index of the desired element after partitioning.
     * @return The k-th smallest element of the vector after partitioning.
     */
    static T quickselect(std::vector<T> &,
                         const std::size_t &, const std::size_t &, const std::size_t &);

    /**
     * @brief Computes the median of a vector using the quickselect algorithm.
     *
     * This method returns the median element of the given vector.
     * @param[in,out] vec The vector to compute the median of.
     * @return The median element of the vector.
     */
    static T median(std::vector<T> &);
};