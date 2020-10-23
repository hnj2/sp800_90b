#include "data.hpp"

data_t build_data(py::bytes samples, int bits_per_symbol, bool truncate) {

    // constructing data
    std::string samples_str = samples;
    data_t data = construct_data_t(samples_str.c_str(), samples_str.length(), bits_per_symbol);

    // some sanity check
	if (data.alph_size <= 1)
		throw std::invalid_argument("Symbol alphabet consists of 1 symbol. No entropy awarded...");

    // truncate to min size
	if (truncate && (data.blen > MIN_SIZE))
        data.blen = MIN_SIZE;

    // warn if not min size
	if (data.len < MIN_SIZE)
        printf("\n*** Warning: data contains less than %d (min size) samples ***\n\n", MIN_SIZE);

    return data;
}

Data::Data(py::bytes samples, int bits_per_symbol, bool truncate)
  : Data(build_data(samples, bits_per_symbol, truncate)) {}

bool Data::iid_tests() const {
    return
        chi_square_tests()
        && lrs_tests()
        && permutation_tests();
}

bool Data::chi_square_tests() const {
    return ::chi_square_tests(this->data.symbols, this->data.len, this->data.alph_size, 0);
}

bool Data::lrs_tests() const {
    return len_LRS_test(this->data.symbols, this->data.len, this->data.alph_size, 0, "Literal");
}

bool Data::permutation_tests() const {
	return ::permutation_tests(&this->data, this->rawmean, this->median, 0);
}

double Data::h_max() const {
    return data.word_size;
}

double Data::h_bitstring_max() const {
    return 1.0;
}

double Data::h_most_common() const {
    return most_common(data.symbols, data.len, data.alph_size, 0, "Literal");
}

double Data::h_bitstring_most_common() const {
    return most_common(data.bsymbols, data.blen, 2, 0, "Bitstring");
}

double Data::h_multi_most_common() const {
    return multi_mcw_test(data.symbols, data.len, data.alph_size, 0, "Literal");
}

double Data::h_bitstring_multi_most_common() const {
    return multi_mcw_test(data.bsymbols, data.blen, 2, 0, "Bitstring");
}

double Data::h_collision() const {
    return collision_test(data.symbols, data.len, 0, "Literal");
}

double Data::h_bitstring_collision() const {
    return collision_test(data.bsymbols, data.blen, 0, "Bitstring");
}

double Data::h_markov() const {
    return markov_test(data.symbols, data.len, 0, "Literal");
}

double Data::h_bitstring_markov() const {
    return markov_test(data.bsymbols, data.blen, 0, "Bitstring");
}

double Data::h_multi_markov() const {
    return multi_mmc_test(data.symbols, data.len, data.alph_size, 0, "Literal");
}

double Data::h_bitstring_multi_markov() const {
    return multi_mmc_test(data.bsymbols, data.blen, 2, 0, "Bitstring");
}

double Data::h_compression() const {
    return compression_test(data.symbols, data.len, 0, "Literal");
}

double Data::h_bitstring_compression() const {
    return compression_test(data.bsymbols, data.blen, 0, "Bitstring");
}

std::pair<double, double> Data::h_t_tuple_and_lrs() const {
    double t_tuple_res = -1.0, lrs_res = -1.0;
    SAalgs(data.symbols, data.len, data.alph_size, t_tuple_res, lrs_res, 0, "Literal");
    return std::make_pair(t_tuple_res, lrs_res);
}

std::pair<double, double> Data::h_bitstring_t_tuple_and_lrs() const {
    double t_tuple_res = -1.0, lrs_res = -1.0;
    SAalgs(data.bsymbols, data.blen, 2, t_tuple_res, lrs_res, 0, "Bitstring");
    return std::make_pair(t_tuple_res, lrs_res);
}

double Data::h_lag_prediction() const {
    return lag_test(data.symbols, data.len, data.alph_size, 0, "Literal");
}

double Data::h_bitstring_lag_prediction() const {
    return lag_test(data.bsymbols, data.blen, 2, 0, "Bitstring");
}

double Data::h_lz78y() const {
    return LZ78Y_test(data.symbols, data.len, data.alph_size, 0, "Literal");
}

double Data::h_bitstring_lz78y() const {
    return LZ78Y_test(data.bsymbols, data.blen, 2, 0, "Bitstring");
}

double Data::h_min_all() const {
    double h = h_max();
    h = min(h, h_most_common());
    if (is_binary)
        h = min(h, h_collision());
    if (is_binary)
        h = min(h, h_markov());
    if (is_binary)
        h = min(h, h_compression());
    auto tup_and_lrs = h_t_tuple_and_lrs();
    if (tup_and_lrs.first >= 0.0)
        h = min(h, tup_and_lrs.first);
    if (tup_and_lrs.second >= 0.0)
        h = min(h, tup_and_lrs.second);
    h = min(h, h_multi_most_common());
    h = min(h, h_lag_prediction());
    h = min(h, h_multi_markov());
    h = min(h, h_lz78y());
    return h;
}

double Data::h_bitstring_min_all() const {
    double h = h_bitstring_max();
    h = min(h, h_bitstring_most_common());
    h = min(h, h_bitstring_collision());
    h = min(h, h_bitstring_markov());
    h = min(h, h_bitstring_compression());
    auto tup_and_lrs = h_bitstring_t_tuple_and_lrs();
    if (tup_and_lrs.first >= 0.0)
        h = min(h, tup_and_lrs.first);
    if (tup_and_lrs.second >= 0.0)
        h = min(h, tup_and_lrs.second);
    h = min(h, h_bitstring_multi_most_common());
    h = min(h, h_bitstring_lag_prediction());
    h = min(h, h_bitstring_multi_markov());
    h = min(h, h_bitstring_lz78y());
    return h;
}

double Data::h_initial() const {
    if (is_binary) return h_min_all();
    return min(h_min_all(), h_conditioned());
}

double Data::h_conditioned() const {
    return data.word_size * h_bitstring_min_all();
}

std::pair<double, double> Data::h_both() const {
    double h_cond = h_conditioned();
    return std::make_pair(min(h_min_all(), h_cond), h_cond);
}


