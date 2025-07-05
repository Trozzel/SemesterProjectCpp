//
// Created by George Ford on 6/30/25.
//

#ifndef NDVECTOR_H
#define NDVECTOR_H

#include <vector>
#include <functional>
#include <span>
#include <array>
#include <fstream>
#include <sstream>
#include <exception>
#include <iostream>

using std::ios;

/**
 * @brief Basic struct for representing 2D dimensions
 * @param row
 * @param col
 */
struct Dims {
	Dims(size_t r, size_t c) : rows(r), cols(c) {}
	size_t rows;
	size_t cols;
	std::string
		operator()()
	{
		return std::to_string(this->rows) + "x" + std::to_string(this->cols);
	}
	// Return String representation
	friend std::ostream& 
		operator<<(std::ostream& out, const Dims& dims)
	{
		out << "(" << dims.rows << "x" << dims.cols << ")";
		return out;
	}
};

/**
 * @brief NDVector - A N-dimensional vector
 */
template<size_t R>
class NDVector {
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

	const std::vector<double>&
		operator[](const size_t row) const
	{
		return _data[row];
	}
	std::vector<double>&
		operator[](const size_t row) 
	{
		return _data[row];
	}

	void
		push_back(size_t row, double val) 
	{
		_data[row].push_back(val);
	}
	Dims
		dims() const 
	{
		return Dims(R, _data[0].size());
	}

	/**
	 * @brief The primary means to create an NDVector is from reading file
	 * contents. This 'constructor' uses basic files without extra symbols.
	 * @param filename
	 * @return an NDVector
	 */
	 static std::unique_ptr<NDVector>
		from_basic_file(std::string filename) 
	{
		constexpr size_t LINE_BUF_LEN = 500;
		std::ifstream datastream(std::move(filename), ios::in | ios::binary);
		if(!datastream.is_open()) {
			throw std::runtime_error("Could not open file, " + filename);
		}
		auto p_ndvec = std::make_unique<NDVector>();
		//NDVector ndvec;
		std::string line{};
		size_t lineno = 0;
		while (std::getline(datastream, line)) {
			std::stringstream ss(line);
			std::string strnum{};
			double num = 0.0;
			for (size_t i = 0; i < R; ++i) {
				ss >> strnum;
				try {
					num = std::stod(strnum);
				}
				catch (std::exception &e) {
					std::cerr << "❌ Failed to convert '" << strnum << "' to a double. " 
						<< e.what() << "\n";
					std::exit(1);
				}
				(*p_ndvec).push_back(i, num);
			}
			lineno++;
		}

		return std::move(p_ndvec);
	}

	/**
	 * @brief Interpolate the original series of arrays via an `NDVector`
	 * @param orig the original data
	 * @param freq the frequency at which the data was recorded
	 * @return Interpolated data at t
	 */ 
	static std::unique_ptr<NDVector>
		interp(std::unique_ptr<NDVector> orig, const double freq)
	{

	}
};

/**
 * @brief Allow various functions to be applied to windows of operations
 * @param data: the data
 * @param window_size: the size of the moving window
 * @param window_op: the operation to be applied to this 2-point window
 */ 
std::vector<double>
	win_fn(std::span<const double> data, 
			size_t window_size,
			std::function<double(double, double)> window_op)
{
	if (window_size > data.size()) {
		throw std::runtime_error("Window size is larger than the original data.");
	}

	std::vector<double> v;
	auto it0 = data.begin();
	auto it1 = data.begin() + window_size;
	while (it1 != data.end()) {
		v.push_back(window_op(*it0, *it1));
		++it0;
		++it1;
	}
	return v;
}

#endif //NDVECTOR_H
