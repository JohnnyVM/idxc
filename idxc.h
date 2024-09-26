#ifndef IDXC_H
#define IDXC_H

/// @brief 
enum idxc_data_t {
    idxc_data_t_unsigned_byte = 0x08,
    idxc_data_t_signed_byte = 0x09,
    idxc_data_t_short = 0x0B,
    idxc_data_t_int = 0x0C,
    idxc_data_t_float = 0x0D,
    idxc_data_t_double = 0x0E
};

/// @brief
///
/// The underlying data is stored as big endian
#define IDXC_MAX_DIMENSIONS 256
struct idxc_t {
    idxc_data_t type;
    unsigned char dimensions;
    unsigned dimension_size[IDXC_MAX_DIMENSIONS];
    /// @brief Array that store the data and the size of the dimensions
    ///
    /// The sizes of this.
    unsigned char data[];
};

#endif

#ifndef IDXC_IMPLEMENTATION
#define IDXC_IMPLEMENTATION

#endif