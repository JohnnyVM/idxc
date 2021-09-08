#include <memory>
#if !defined(IDX_HPP)
#define IDX_HPP

#include <string>
#include <cstdint>
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
	explicit Idx(const char* filename);
	explicit Idx(std::string& filename);
	~Idx();

	explicit operator uint8_t*() noexcept;
	Idx operator[](size_t position);

	enum idx_type_data type;
	size_t number_of_elements;
	std::vector<uint32_t>dimension;
	Idx slice(size_t origin, size_t end);

	private:
	size_t element_size;
	explicit Idx(std::shared_ptr<struct idx_memory>);
	void* payload; // chunks of memory in big endian format
};

#endif
