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

		enum idx_type_data type;
		size_t number_of_elements;
		std::vector<uint32_t>dimension;

		private:
		std::vector<uint8_t> byte;
	};

#endif
