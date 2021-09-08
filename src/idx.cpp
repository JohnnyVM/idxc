#include <cstdint>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <string.h>

extern "C" {
#include "idx_error_c.h"
#include "idx_memory_c.h"
#include "idx_result_c.h"
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

	std::shared_ptr<struct idx_memory> mem(memory.memory, idx_memory_free);

	// Pass from C to C++
	element_size = mem->element_size;
	payload = malloc(mem->number_of_elements * mem->element_size);
	memcpy(payload, mem->element, mem->number_of_elements * mem->element_size);

	number_of_elements = mem->number_of_elements;
	type = (enum idx_type_data)mem->type;
	dimension.reserve(mem->number_of_dimensions);
	for(size_t i = 0; i < dimension.size(); i++) {
		dimension[i] = mem->dimension[i];
	}
}

Idx::~Idx() {
	free(payload);
}

Idx::operator uint8_t*() noexcept {
	if(type != UNSIGNED_8_INT) {
		return nullptr;
	}

	return (uint8_t*)payload;
}

Idx Idx::slice(size_t begin, size_t end) {

	if(begin > end || begin > number_of_elements) { throw std::invalid_argument("Invalid index"); }
	Idx out = *this;

	out.payload = malloc(out.number_of_elements * out.element_size);

	memcpy(out.payload, (uint8_t*)this->payload + begin * element_size, element_size * (end - begin));

	return out;
}

Idx Idx::operator[](size_t position) {
	return slice(position, position + 1);
}

