#ifndef DATA_HPP
#define DATA_HPP

#include <pybind11/pybind11.h>

#include <utility>

#include "nist.hpp"

namespace py = pybind11;

class Data {
private:
    data_t data;

public:
    const double rawmean;
    const double median;
    const bool   is_binary;

private:
    Data(data_t data, Stats stats)
      : data(data), 
        rawmean(stats.rawmean),
        median(stats.median),
        is_binary(data.alph_size == 2) {}

    Data(data_t data) : Data(data, Stats(data)) {}

public:
    Data(py::bytes samples, int bits_per_symbol = 8, bool truncate = false);
    ~Data() { free_data(&data); }


    // distribution tests
    bool iid_tests() const;

    bool chi_square_tests() const;
    bool lrs_tests() const;
    bool permutation_tests() const;

    // entropy estimates
    std::pair<double, double> h_both() const;

    double h_initial() const;
    double h_conditioned() const;

    double h_max() const;
    double h_bitstring_max() const;

    double h_most_common() const;
    double h_bitstring_most_common() const;

    double h_collision() const;
    double h_bitstring_collision() const;

    double h_markov() const;
    double h_bitstring_markov() const;

    double h_compression() const;
    double h_bitstring_compression() const;

    std::pair<double, double> h_t_tuple_and_lrs() const;
    std::pair<double, double> h_bitstring_t_tuple_and_lrs() const;

    double h_multi_most_common() const;
    double h_bitstring_multi_most_common() const;

    double h_lag_prediction() const;
    double h_bitstring_lag_prediction() const;

    double h_multi_markov() const;
    double h_bitstring_multi_markov() const;

    double h_lz78y() const;
    double h_bitstring_lz78y() const;

    double h_min_all() const;
    double h_bitstring_min_all() const;

};

#endif
