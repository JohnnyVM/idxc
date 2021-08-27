#include <cstdint>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <memory>

extern "C" {
#include "idx_error_c.h"
#include "idx_memory_c.h"
}

#include "idx.hpp"

Idx::Idx(std::string& filename) {

	Idx{filename.c_str()};
}

Idx::Idx(const char* filename) {
	struct idx_result memory = idx_memory_from_filename(filename);
	if(memory.error) {
		if(memory.error_message) { /* TODO */}
		throw memory.error;
	}

	std::shared_ptr<struct idx_memory> idx_ptr(memory.memory, idx_memory_free);

	// Pass from C to C++
	number_of_elements = idx_ptr->number_of_elements;
	type = (enum idx_type_data)idx_ptr->type;
	dimension.reserve(idx_ptr->number_of_dimensions);
	for(size_t i = 0; i < dimension.size(); i++) {
		dimension[i] = idx_ptr->dimension[i];
	}
}

