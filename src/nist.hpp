#ifndef NIST_HPP
#define NIST_HPP

# ifndef NO_NIST_DEFS
#   define MIN_SIZE 1000000
#   define min(a,b) \
        ({ __typeof__ (a) _a = (a); \
           __typeof__ (b) _b = (b); \
            _a < _b ? _a : _b; })

typedef unsigned char byte;
typedef struct data_t data;

struct data_t {
    int word_size;      // bits per symbol
    int alph_size;      // symbol alphabet size
    byte maxsymbol;     // the largest symbol present in the raw data stream
    byte *rawsymbols;   // raw data words
    byte *symbols;      // data words
    byte *bsymbols;     // data words as binary string
    long len;       // number of words in data
    long blen;      // number of bits in data
};

# endif

void SAalgs(const byte data[], const long len, const int alph_size, double &t_tuple_res, double &lrs_res, const int verbose, const char *label);

double most_common(byte* data, const long len, const int alph_size, const int verbose, const char *label);
double multi_mcw_test(byte* data, const long len, const int alph_size, const int verbose, const char *label);
double collision_test(byte* data, const long len, const int verbose, const char *label);
double markov_test(byte* data, const long len, const int verbose, const char *label);
double multi_mmc_test(byte* data, const long len, const int alph_size, const int verbose, const char *label);
double compression_test(byte* data, const long len, const int verbose, const char *label);
double lag_test(byte* data, const long len, const int alph_size, const int verbose, const char *label);
double LZ78Y_test(byte* data, const long len, const int alph_size, const int verbose, const char *label);

data_t construct_data_t(const char * samples, int sample_length, int bits_per_symbol);
void free_data(data_t * data);

bool len_LRS_test(const byte data[], const int length, const int k, const int verbosity, const char* label);

bool chi_square_tests(const byte data[], const int length, int alph_size, const int verbosity);

bool permutation_tests(const data_t * data, double rawmean, double median, int verbosity);

class Stats {
public:
    double rawmean, median;
    Stats(data_t &data);
};

data_t construct_data_t(
    const char *symbols,
    unsigned long symbols_len,
    int bits_per_word
);

#endif
