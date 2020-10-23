#include <pybind11/pybind11.h>

#include "data.hpp"

namespace py = pybind11;
using namespace py::literals;

PYBIND11_MODULE(sp800_90b, m) {

    m.doc() = "SP 800-90B entropy assesment, using the nist-provided implementation.";

    py::class_<Data>(m, "Data", "Samples that can be assessed for entropy.")

        .def(
            py::init<py::bytes, int, bool>(),
            "Constructs entropy assessment sample data.",
            "samples"_a, "bits_per_symbol"_a = 8, "truncate"_a = false
        )

        .def_readonly("is_binary", &Data::is_binary, "Whether the data only consisty of two different symbols.")

        .def_readonly("rawmean", &Data::rawmean, "Statistical mean value of the data.")
        .def_readonly("median", &Data::median, "Statistical median value of the data.")

        .def("iid_tests", &Data::iid_tests, "Tests if the data is independently and identically distributed using `chi_square_test`, `lrs_tests` and `permutation_tests`.")
        .def("chi_square_tests", &Data::chi_square_tests, "Tests if the data is independently and identically distributed using Pearson's chi-squared test.")
        .def("lrs_tests", &Data::lrs_tests, "Tests if the data is independently and identically distributed using an LSR test.")
        .def("permutation_tests", &Data::permutation_tests, "Tests if the data is independently and identically distributed using permutations.")

        .def("h_initial", &Data::h_initial, "Computes the initial entropy estimate (Section 3.1.3).")
        .def("h_conditioned", &Data::h_conditioned, "Computes the entropy estimate foa a conditioned seqauential dataset (Section 3.1.5.2).")
        .def("h_both", &Data::h_both, "Computes both entropy estimates.")

        .def("h_min_all", &Data::h_min_all, "Computes the minimum of the entropy estimates for the original data.")

        .def("h_max", &Data::h_max, "The maximal entropy for the dataset (word_size).")
        .def("h_most_common", &Data::h_most_common, "Estimates the entropy with the most common value (Section 3.6.1).")
        .def("h_collision", &Data::h_collision, "Estimates the entropy with a collision test (Section 6.3.2).")
        .def("h_markov", &Data::h_markov, "Estimates the entropy with a markov test (Section 6.3.3).")
        .def("h_compression", &Data::h_compression, "Estimates the entropy with a compression test (Section 6.3.4).")
        .def("h_t_tuple_and_lrs", &Data::h_t_tuple_and_lrs, "Estimates the entropy with a t-tuple and a lrs test (Sections 6.3.5 and 6.3.6).")
        .def("h_multi_most_common", &Data::h_multi_most_common, "Estimates the entropy with a multi most common in window test (Section 6.3.7).")
        .def("h_lag_prediction", &Data::h_lag_prediction, "Estimates the entropy with a lag prediction test (Section 6.3.8).")
        .def("h_multi_markov", &Data::h_multi_markov, "Estimates the entropy with a multi markov model with counting test (Section 6.3.9).")
        .def("h_lz78y", &Data::h_lz78y, "Estimates the entropy with a LZ78Y test (Section 6.3.10).")


        .def("h_bitstring_min_all", &Data::h_bitstring_min_all, "Computes the minimum of the entropy estimates for the data in bitstring form.")

        .def("h_bitstring_max", &Data::h_bitstring_max, "The maximal entropy for the bitstring data (is 1.0).")
        .def("h_bitstring_most_common", &Data::h_bitstring_most_common, "Estimates the entropy of the data as a bistring with the most common value (Section 3.6.1).")
        .def("h_bitstring_collision", &Data::h_bitstring_collision, "Estimates the entropy of the data as a bistring with a collision test (Section 6.3.2).")
        .def("h_bitstring_markov", &Data::h_bitstring_markov, "Estimates the entropy of the data as a bistring with a markov test (Section 6.3.3).")
        .def("h_bitstring_compression", &Data::h_bitstring_compression, "Estimates the entropy of the data as a bistring with a compression test (Section 6.3.4).")
        .def("h_bitstring_t_tuple_and_lrs", &Data::h_bitstring_t_tuple_and_lrs, "Estimates the entropy of the data as a bistring with a t-tuple and a lrs test (Sections 6.3.5 and 6.3.6).")
        .def("h_bitstring_multi_most_common", &Data::h_bitstring_multi_most_common, "Estimates the entropy of the data as a bistring with a multi most common in window test (Section 6.3.7).")
        .def("h_bitstring_lag_prediction", &Data::h_bitstring_lag_prediction, "Estimates the entrop of the data as a bistringy with a lag prediction test (Section 6.3.8).")
        .def("h_bitstring_multi_markov", &Data::h_bitstring_multi_markov, "Estimates the entropy of the data as a bistring with a multi markov model with counting test (Section 6.3.9).")
        .def("h_bitstring_lz78y", &Data::h_bitstring_lz78y, "Estimates the entropy of the data as a bistring with a LZ78Y test (Section 6.3.10).")
    ;

}
