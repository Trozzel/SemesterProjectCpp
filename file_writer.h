#ifndef _FILE_WRITER_H
#define _FILE_WRITER_H

#include <cstddef>
#include <fstream>
#include <ios>
#include <memory>

#include "interpolate.h"

using std::unique_ptr;

namespace fs = std::filesystem;

/// Create resultant filenames
/// @og_filepath:  "Data/sensors-2018.12.26.txt"
static fs::path create_core_filename(const fs::path& og_filepath,
                                     size_t file_count)
{
    fs::path parent_path = og_filepath.parent_path(); // "Data"
    std::string fname = og_filepath.stem().string();  // "sensors-2018.12.26"
    std::string extension = og_filepath.extension().string(); // ".txt"
    std::string core_fname =
        fname + "-core-" + std::to_string(file_count) + extension;

    return parent_path / core_fname;
}

/**
 * @brief write the interpolated data to `Rows` amount of files
 * @param p_data
 * @param time_period the time between each data point
 * @param og_filename original filename
 */
template <size_t Rows>
void write_to_files(const std::unique_ptr<InterpMat<Rows>>& p_data,
                    const double time_period,
                    const fs::path& og_filepath)
{
    for (size_t vec_idx = 0; vec_idx < Rows; ++vec_idx)
    {
        std::vector<InterpPt> v = (*p_data)[vec_idx];
        auto new_fpath = create_core_filename(og_filepath, vec_idx);
        auto ofs = std::ofstream(new_fpath, std::ios::trunc);
        double pt_idx = 0.0;
        for (const auto& pt : v)
        {
            ofs << std::right << std::fixed
                << std::setw(8)
                /*       0 */
                << std::setprecision(0) << pt_idx++ *
                time_period
                /* <= x <= */
                << " <= x <= " << std::right << std::fixed
                << std::setw(8)
                /*      30 */
                << std::setprecision(0) << pt_idx *
                time_period
                /*    ;y = */
                << " ; y = " << std::right << std::setw(12)
                << std::fixed
                /* 61.0000 */
                << std::setprecision(4) << pt.b << " +"
                << std::right
                /*  0.6333 */
                << std::setw(12) << std::fixed << std::setprecision(4) << pt.m
                << " x ; interpolation" << std::endl;
        }
    }
}

#endif // _FILE_WRITER_H
