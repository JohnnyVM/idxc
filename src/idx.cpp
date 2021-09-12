#include <cstdint>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <string.h>
#include <cstddef>

extern "C" {
#include "idx_error_c.h"
#include "idx_memory_c.h"
#include "idx_result_c.h"
#include "idx_type_data_c.h"
}

#include "idx.hpp"

Idx::Idx(const Idx& ctor) :
	type(ctor.type),
	number_of_elements(ctor.number_of_elements),
	element_size(ctor.element_size),
	type_size(ctor.type_size)
{

	payload = std::make_unique<std::uint8_t[]>(ctor.number_of_elements * ctor.element_size);
	memcpy(payload.get(), ctor.payload.get(), ctor.number_of_elements * ctor.element_size);
	dimension = ctor.dimension;
}

Idx::Idx(std::string& filename) : Idx{filename.c_str()} {}

Idx::~Idx() {} // Created as tests

Idx::Idx(Idx* parent, size_t begin, size_t end) {

	if(begin > end || begin > parent->number_of_elements) { throw std::invalid_argument("Invalid index"); }

	type = parent->type;
	dimension = parent->dimension;
	number_of_elements = end - begin;
	element_size = parent->element_size;
	type_size = parent->type_size;

	payload = std::make_unique<std::uint8_t[]>(number_of_elements * element_size);

	memcpy(payload.get(), parent->payload.get() + begin * element_size, element_size * number_of_elements);
}

Idx::Idx(const char* filename) {
	struct idx_result memory = idx_memory_from_filename(filename);
	if(memory.error) {
		if(memory.error_message) { /* TODO */}
		throw memory.error;
	}

	struct idx_memory* mem = memory.memory;

	// Pass from C to C++
	element_size = mem->element_size;
	payload = std::make_unique<std::uint8_t[]>(mem->number_of_elements * mem->element_size);
	memcpy(payload.get(), mem->element, mem->number_of_elements * mem->element_size);

	number_of_elements = mem->number_of_elements;
	type = mem->type;
	type_size = idx_type_data_size(type);
	dimension.reserve(mem->number_of_dimensions);
	for(size_t i = 0; i < mem->number_of_dimensions; i++) {
		dimension.push_back(mem->dimension[i]);
	}

	idx_result_free(memory);
}

Idx::operator uint8_t*() {
	if(type != UNSIGNED_8_INT) {
		return nullptr;
	}

	return payload.get();
}

Idx Idx::operator[](size_t position) {
	return slice(position, position + 1);
}

Idx Idx::slice(size_t begin, size_t end) {
	return Idx(this, begin, end);
}
