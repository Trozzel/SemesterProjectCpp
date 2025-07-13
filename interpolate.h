#ifndef _INTERPOLATE_H
#define _INTERPOLATE_H

#include "NDVector.h"

/** INTERPOLATION POINT
 * @brief abstraction of each interpolated point
 * @param m: slope
 * @param b: y-int
 * @param idx: relative index of `this` data point
 * @note the `interpolate` function returns `N` vectors of these points
 */
struct InterpPt
{
    double m;
    double b;
    size_t idx;
};

// Format `InterpPt` for `std::format` display
template <>
struct std::formatter<InterpPt> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const InterpPt& pt, FormatContext& ctx) const
    {
        return std::format_to(ctx.out(),
                              "InterpPt(m: {:5.3f}, b: {:.3f}, idx: {:d})",
                              pt.m,
                              pt.b,
                              pt.idx);
    }
};

using std::unique_ptr;

// Return type of `interpolate`
/******************************************************************************/
template <size_t N>
using InterpMat = std::array<std::vector<InterpPt>, N>;

/** INTERPOLATE
 * @brief Applies linerar interpolation to `N` time-based data points
 * @param p_ndvec: Original data as an `NDVector`
 * @param time_period: time "distance" between each data point
 * @return N x L-1 matrix of `InterpPt` that contain the data required for the
 * output files
 */
template <size_t N>
unique_ptr<InterpMat<N>> interpolate(const unique_ptr<NDVector<N>>& p_ndvec,
                                     double time_period = 30.0)
{
    // Initiate return object
    auto p_result = std::make_unique<InterpMat<N>>();

    for (auto row_num = 0; row_num < N; ++row_num)
    {
        const std::vector<double>& vec = p_ndvec->vec_at(row_num);
        auto it0 = vec.cbegin();
        auto it1 = it0 + 1;
        for (size_t idx = 0; it1 != vec.cend(); ++idx, ++it0, ++it1)
        {
            auto m = (*it1 - *it0) / time_period;
            // b = y - m*x
            auto b = *it0 - (m * time_period * static_cast<double>(idx));
            (*p_result)[row_num].push_back(InterpPt(m, b, idx));
        }
    }
    return std::move(p_result);
}

#endif // _INTERPOLATE_H

