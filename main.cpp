#include <iostream>
#include <string>
#include <iterator>

#include "NDVector.h"

constexpr size_t ROWS = 4;
constexpr double TIME_PERIOD = 30.0; // seconds

int main(int argc, char** argv) {
	std::string filename {"Data/play.txt"};
	if (argc > 1) {
		filename = argv[1];
	}
	else {
		std::cout << "No file was passed. Using " << filename << std::endl;
	}

	auto p_ndvec = NDVector<ROWS>::from_basic_file(filename);

	// Create a labmda function that interpolates between two values by the
	// perior of 30 seconds.
	auto f_interp = [=] (double n1, double n2) -> double {
		return (n2 - n1) / TIME_PERIOD;
	};

	std::cout << "dims string: " << p_ndvec->dims() << "\n";

	auto p_ndinterp = std::make_unique<NDVector<ROWS>>();
	size_t vec_idx = 0;
	for (const auto& argv : *p_ndvec) {
		(*p_ndinterp)[vec_idx++] = win_fn(argv, 2, f_interp);
	}

	std::cout << "length of interpolated: " <<
		std::distance((*p_ndvec)[0].begin(), (*p_ndvec)[0].end()) << "\n"; 
}

