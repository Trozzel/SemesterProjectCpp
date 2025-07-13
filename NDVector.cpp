#include "NDVector.h"

/**
 * @brief apply a binary function to an array of elements and return a vector
 * of the results
 * @param data
 * @param window_size how many elements to include
 * @param window_op operation to apply on the elements
 */
std::vector<double> win_fn(std::span<const double> data,
                           std::function<double(double, double)> window_op)
{

    if (data.size() < 2)
    {
        throw std::runtime_error("Input array must be larger than 2.");
    }

    std::vector<double> v;
    auto it0 = data.begin();
    auto it1 = it0 + 1; // data.begin() - 1;
    for (; it1 != data.end(); ++it0, ++it1)
    {
        v.push_back(window_op(*it0, *it1));
    }
    return v;
}
