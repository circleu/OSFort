#include "pcie.h"


uint64_t pcie_array_to_uint64(uint8_t ptr[], size_t size) {
    size_t ret = 0;

    for (size_t i = 0; i < size; i++) {
        ret += ptr[i] << (i * 8);
    }
    return ret;
}