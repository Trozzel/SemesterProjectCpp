#include <print>
#include <ranges>
#include <string>

#include "NDVector.h"
//#include "file_writer.h"
#include "interpolate.h"
#include "least-squares.h"

constexpr size_t ROWS = 4;
constexpr double TIME_PERIOD = 30.0; // seconds

// Create a labmda function that interpolates between two values by the
// period of 30 seconds.
constexpr auto F_INTERP = [](double n0, double n1) -> double
{ return (n1 - n0) / TIME_PERIOD; };

int main(int argc, char** argv)
{
    std::string filename{"Data/play.txt"};
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        std::println("⚠️ No file was passed. Using {}", filename);
    }

    auto p_ndvec = NDVector<ROWS>::from_basic_file(filename);

    std::cout << "Dimensions: " << p_ndvec->dims() << "\n";
    std::println("Intperpolating...");

    // Obtain table of interpolated data
    auto p_res = interpolate(p_ndvec, TIME_PERIOD);
    // Write the final results to file
    // write_to_files(p_res, TIME_PERIOD, filename);

    // Perform Least Squares fit
	auto p_ls = least_squares(p_ndvec);
	for(const auto [i, ls] : std::views::zip(std::views::iota(0) ,(*p_ls))) 
	{
		std::println("LeastSquares[{}]: {}", i, ls);
	}
}
