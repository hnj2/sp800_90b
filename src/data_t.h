#include <shared/utils.h>

// Construct a data_t from memory
data_t construct_data_t(
    const char *symbols,
    unsigned long symbols_len,
    int bits_per_word
) {

	if (symbols_len == 0)
        throw std::invalid_argument("Need more than zero symbols!");

    data_t data;

    ////////////////////////////////////////////////////
    // init symbols
    data.len = symbols_len;
	data.symbols = (byte*) malloc(data.len);
	if (data.symbols == NULL) {
        free_data(&data);
        throw std::runtime_error("Failure to initialize memory for symbols!");
	}
    memcpy(data.symbols, symbols, data.len);

    ////////////////////////////////////////////////////
    // init wordsize
    data.word_size = bits_per_word;

    // check bits per word with the actual symbols
	byte datamask = 0;
	byte curbit = 0x80;

	for(int i = 0; i < data.len; i++) {
		datamask = datamask | data.symbols[i];
	}

    int calculated_wordsize;
	for(calculated_wordsize=8; (calculated_wordsize>0) && ((datamask & curbit) == 0); calculated_wordsize--) {
		curbit = curbit >> 1;
	}

	if( calculated_wordsize < data.word_size ) {
		printf("Warning: Symbols appear to be narrower than described.\n");
	} else if( calculated_wordsize > data.word_size ) {
		free_data(&data);
        throw std::invalid_argument("Incorrect bit width specification: Data does not fit within described bit width.\n");
	}

    ////////////////////////////////////////////////////
    // init rawsymbols
	data.rawsymbols = (byte*) malloc(data.len);
	if(data.rawsymbols == NULL){
		free_data(&data);
        throw std::runtime_error("Failure to initialize memory for rawsymbols!");
	}
	memcpy(data.rawsymbols, data.symbols, data.len);

    ////////////////////////////////////////////////////
    // init max symbols and create symbol map down table
	data.maxsymbol = 0;
	int max_symbols = 1 << data.word_size;
	int symbol_map_down_table[max_symbols];

	// create symbols (samples) and check if they need to be mapped down
	data.alph_size = 0;
	memset(symbol_map_down_table, 0, max_symbols*sizeof(int));
	int mask = max_symbols-1;
	for(int i = 0; i < data.len; i++){ 
		data.symbols[i] &= mask;
		if(data.symbols[i] > data.maxsymbol) data.maxsymbol = data.symbols[i];
		if(symbol_map_down_table[data.symbols[i]] == 0) symbol_map_down_table[data.symbols[i]] = 1;
	}

	for(int i = 0; i < max_symbols; i++){
		if(symbol_map_down_table[i] != 0) symbol_map_down_table[i] = (byte)data.alph_size++;
	}

    ////////////////////////////////////////////////////
	// create bsymbols (bitstring) using the non-mapped data
	data.blen = data.len * data.word_size;
	if(data.word_size == 1) data.bsymbols = data.symbols;
	else{
		data.bsymbols = (byte*)malloc(data.blen);
		if(data.bsymbols == NULL){
			throw std::runtime_error("failure to initialize memory for bsymbols!");
			free_data(&data);
		}

		for(int i = 0; i < data.len; i++){
			for(int j = 0; j < data.word_size; j++){
				data.bsymbols[i*data.word_size+j] = (data.symbols[i] >> (data.word_size-1-j)) & 0x1;
			}
		}
	}

    ////////////////////////////////////////////////////
	// map down symbols if less than 2^bits_per_word unique symbols
	if(data.alph_size < data.maxsymbol + 1){
		for(int i = 0; i < data.len; i++) data.symbols[i] = (byte)symbol_map_down_table[data.symbols[i]];
	} 

	return data;
}

