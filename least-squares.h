#ifndef _LEAST_SQUARES_H
#define _LEAST_SQUARES_H

#include "NDVector.h"
#include <format>
#include <memory>
#include <ranges>

using std::unique_ptr;

/** LEAST SQUARES
 * @brief representation of 1st order polynomial Least Squares across entire
 * array
 * @param c0: 1st contstant in the polynomial
 * @param c1: 2nd contstant in the polynomial
 */
struct LeastSquare
{
    double c0;
    double c1;
};

// Format `LeastSquares` for `std::format` display
template <>
struct std::formatter<LeastSquare> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const LeastSquare& ls, FormatContext& ctx) const
    {
        return std::format_to(
            ctx.out(), "LeastSquares(c0: {:5.3}, c1: {:5.3})", ls.c0, ls.c1);
    }
};

/** CALCULATE C1
 * param c: summation of f(t) from t0 to tN
 * param k: summataion of 1 from t0 to tN
 * param alpha: summation of t from t0 to tN
 * param c1: derived from partially solving for
 */
static double calc_constant_0(double c, double k, double alpha, double c1);

/** CALCULATE C0
 * param k: summataion of 1 from t0 to tN
 * double d: sumimation of f(t) * t from t0 to tN
 * param c: summation of f(t) from t0 to tN
 * param alpha: summation of t from t0 to tN
 * param beta: summation of t^2 from t0 to tN
 */
static double
calc_constant_1(double k, double d, double c, double alpha, double beta);

/** LEAST SQUARES
 * @param p_data: unique_ptr of the original data
 * @param time_period: period between each data point
 * @return N LeastSquare structs
 */
template <size_t N>
unique_ptr<std::array<LeastSquare, N>>
least_squares(const unique_ptr<NDVector<N>>& p_data, double time_period = 30.0)
{
    double alpha = 0.0;
    double beta = 0.0;
    double c = 0.0;
    double d = 0.0;
    double k = static_cast<double>((*p_data)[0].size()) + 1.0;

    // Prep results
    auto p_result = std::make_unique<std::array<LeastSquare, N>>();

    // 1. Iterate through the array of vectors
    for (auto [row_idx, v] :
         std::ranges::views::zip(std::views::iota(0), (*p_data)))
    {
        // 2. Iterate through the vectors
        for (int i = 0; i < v.size(); ++i)
        {
            auto t = (i * time_period);
            alpha += t;
            beta += t * t;
            auto y = v[i];
            c += y;
            d += y * t;
        }
        // Calculate the Leasst Squares coefficients
        auto c1 = calc_constant_1(k, d, c, alpha, beta);
        auto c0 = calc_constant_0(c, k, alpha, c1);
        (*p_result)[row_idx] = LeastSquare(c0, c1);
    }
    return std::move(p_result);
}

// OBTAIN c0 AND c1
/******************************************************************************/
static double calc_constant_0(double c, double k, double alpha, double c1)
{
    return (c / k) - (alpha / k) * c1;
}

static double
calc_constant_1(double k, double d, double c, double alpha, double beta)
{
    return ((k * d) - (c * alpha)) / ((k * beta) - (alpha * alpha));
}

#endif // _LEAST_SQUARES_H
