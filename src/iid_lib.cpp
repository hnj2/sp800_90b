#include <shared/most_common.h>
#include <shared/lrs_test.h>
#include <iid/permutation_tests.h>
#include <iid/chi_square_tests.h>
#include <omp.h>
#include <getopt.h>
#include <limits.h>

#include <pybind11/pybind11.h>

#include "data_t.h"

namespace py = pybind11;

int verbosity = 0;

void ea_iid(py::bytes samples, int bits_per_symbol = 8, bool truncate = false, bool conditioned = false) {

	if (verbosity > 0) {
		printf("Loading data!\n");
	}

    std::string samples_str = samples;
    data_t data = construct_data_t(samples_str.c_str(), samples_str.length(), bits_per_symbol);

	if (verbosity > 0)
        printf("Loaded %ld samples of %d distinct %d-bit-wide symbols\n",
            data.len, data.alph_size, data.word_size);

	if (data.alph_size <= 1) {
		throw std::invalid_argument("Symbol alphabet consists of 1 symbol. No entropy awarded...");
	}

	if (truncate && (data.blen > MIN_SIZE)) {
	    if (verbosity > 0)
            printf("Truncating data (not using %ld bits).\n", data.blen - MIN_SIZE);
        data.blen = MIN_SIZE;
    }

	if ((verbosity > 0) && ((data.alph_size > 2) || conditioned))
        printf("Number of Binary samples: %ld\n", data.blen);

	if (data.len < MIN_SIZE)
        printf("\n*** Warning: data contains less than %d samples ***\n\n", MIN_SIZE);

	if (verbosity > 0) {
		if (data.alph_size < (1 << data.word_size))
            printf("\nSamples have been translated\n");
	}

	double rawmean, median;

	// Calculate baseline statistics
	int alphabet_size = data.alph_size;
	int sample_size = data.len;

	printf("Calculating baseline statistics...\n");
	calc_stats(&data, rawmean, median);

	if (verbosity > 0) {
		printf("\tRaw Mean: %f\n", rawmean);
		printf("\tMedian: %f\n", median);
		printf("\tBinary: %s\n\n", (alphabet_size == 2 ? "true" : "false"));
	}

	double H_original = data.word_size;
	double H_bitstring = 1.0;

	// Compute the min-entropy of the dataset
	if (!conditioned) {
		H_original = most_common(data.symbols, sample_size, alphabet_size, verbosity, "Literal");
	}

	if (((data.alph_size > 2) || conditioned)) {
		H_bitstring = most_common(data.bsymbols, data.blen, 2, verbosity, "Bitstring");
	}

    if (verbosity <= 1) {
        if (conditioned) {
            printf("h': %f\n", H_bitstring);
        } else {
            printf("H_original: %f\n", H_original);
            if (data.alph_size > 2) {
                printf("H_bitstring: %f\n", H_bitstring);
                printf("min(H_original, %d X H_bitstring): %f\n", data.word_size, min(H_original, data.word_size*H_bitstring));
            }
        }
    } else {
        double h_assessed = data.word_size;

        if((data.alph_size > 2) || conditioned) {
                h_assessed = min(h_assessed, H_bitstring * data.word_size);
                printf("H_bitstring = %.17g\n", H_bitstring);
        }

        if(!conditioned) {
                h_assessed = min(h_assessed, H_original);
                printf("H_original: %.17g\n", H_original);
        }

        printf("Assessed min entropy: %.17g\n", h_assessed);
    }

	printf("\n");

	// Compute chi square stats
	bool chi_square_test_pass = chi_square_tests(data.symbols, sample_size, alphabet_size, verbosity);

	if(chi_square_test_pass){
		printf("** Passed chi square tests\n\n");
	}else{
		printf("** Failed chi square tests\n\n");
	}

	// Compute length of the longest repeated substring stats
	bool len_LRS_test_pass = len_LRS_test(data.symbols, sample_size, alphabet_size, verbosity, "Literal");

	if(len_LRS_test_pass){
		printf("** Passed length of longest repeated substring test\n\n");
	}else{
		printf("** Failed length of longest repeated substring test\n\n");
	}

	// Compute permutation stats
	bool perm_test_pass = permutation_tests(&data, rawmean, median, verbosity);

	if(perm_test_pass){
		printf("** Passed IID permutation tests\n\n");
	}else{
		printf("** Failed IID permutation tests\n\n");
	}

	free_data(&data);
	return;
}

using namespace pybind11::literals;

PYBIND11_MODULE(sp800_90b, m) {
    m.doc() = "SP 800-90B entropy assesment, using the inist-provided implementation.";

    m.def(
        "entropy_assesment_iid", &ea_iid,
        "Tests whether the data is independent and identically distributed.",
        "data"_a, "bits_per_symbol"_a = 8, "truncate"_a = false, "conditioned"_a = false
    );
}
