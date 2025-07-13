//
// Created by George Ford on 6/30/25.
//

#ifndef NDVECTOR_H
#define NDVECTOR_H

#include <array>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <span>
#include <sstream>
#include <vector>

using std::ios;

/**
 * @brief Basic struct for representing 2D dimensions
 * @param row
 * @param col
 */
struct Dims
{
    Dims(size_t r, size_t c) : rows(r), cols(c) {}
    size_t rows;
    size_t cols;
    std::string operator()()
    {
        return std::to_string(this->rows) + "x" + std::to_string(this->cols);
    }
    // Return String representation
    friend std::ostream& operator<<(std::ostream& out, const Dims& dims)
    {
        out << "(" << dims.rows << "x" << dims.cols << ")";
        return out;
    }
};

#include <format>
template <>
struct std::formatter<Dims> : std::formatter<string>
{
    template <typename FormatContext>
    auto format(const Dims& d, FormatContext& ctx)
    {
        return std::format("({}x{})", d.rows, d.cols);
    }
};

/**
 * @brief NDVector: A N-dimensional vector
 * @param _data: std::array<std::vector<double>,
 */
template <size_t R>
class NDVector
{
  private:
    std::array<std::vector<double>, R> _data;

  public:
    auto begin() const
    {
        return _data.begin();
    }
    auto end() const
    {
        return _data.end();
    }
    auto begin()
    {
        return _data.begin();
    }
    auto end()
    {
        return _data.end();
    }
    const std::vector<double>& vec_at(size_t row) const
    {
        return _data[row];
    }
    std::vector<double>& vec_at(size_t row)
    {
        return _data[row];
    }
    const std::vector<double>& operator[](const size_t row) const
    {
        return _data[row];
    }
    std::vector<double>& operator[](const size_t row)
    {
        return _data[row];
    }
    void push_back(size_t row, double val)
    {
        _data[row].push_back(val);
    }
    Dims dims() const
    {
        return Dims(R, _data[0].size());
    }

    /**
     * @brief The primary means to create an NDVector is from reading file
     * contents. This 'constructor' uses basic files without extra symbols.
     * @param filename
     * @return an NDVector
     */
    static std::unique_ptr<NDVector> from_basic_file(std::string filename)
    {
        constexpr size_t LINE_BUF_LEN = 500;
        std::ifstream ifs(std::move(filename), ios::in | ios::binary);
        if (!ifs.is_open())
        {
            throw std::runtime_error("Could not open file, " + filename);
        }
        auto p_ndvec = std::make_unique<NDVector>();
        std::string line{};
        size_t lineno = 0;
        while (std::getline(ifs, line))
        {
            std::stringstream ss(line);
            std::string strnum{};
            double num = 0.0;
            for (size_t i = 0; i < R; ++i)
            {
                try
                {
                    ss >> num;
                }
                catch (std::exception& e)
                {
                    std::cerr << "❌ Failed to convert '" << ss.str()
                              << "' to a double. " << e.what() << "\n";
                    std::exit(1);
                }
                p_ndvec->push_back(i, num);
            }
            lineno++;
        }

        return std::move(p_ndvec);
    }
};

/**
 * @brief Allow various functions to be applied to windows of operations
 * @param data: the data
 * @param window_size: the size of the moving window
 * @param window_op: the operation to be applied to this 2-point window
 */
std::vector<double> win_fn(std::span<const double> data,
                           size_t window_size,
                           std::function<double(double, double)> window_op);

#endif // NDVECTOR_H
