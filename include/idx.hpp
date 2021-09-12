#include <memory>
#if !defined(IDX_HPP)
#define IDX_HPP

#include <string>
#include <cstdint>
#include <cstddef>
#include <vector>

extern "C" {
#include "idx_type_data_c.h"
}

/** base is idx_memory but no for the payload
 * in C we will use vector
 * */
class Idx
{
	public:
	Idx(const Idx&);// ctor
	virtual ~Idx();
	explicit Idx(const char* filename);
	explicit Idx(std::string& filename);
	explicit Idx(Idx* parent, size_t begin, size_t end);

	explicit operator uint8_t*();
	Idx operator[](size_t position);

	enum idx_type_data type;
	size_t number_of_elements;
	std::vector<uint32_t>dimension;
	Idx slice(size_t origin, size_t end);
	size_t element_size;
	size_t type_size;
	std::unique_ptr<std::uint8_t[]> payload; // chunks of memory in big endian format

	private:
	explicit Idx(std::shared_ptr<struct idx_memory>);
};

#endif
