#include "acpi.h"


void* acpi_get_table(ACPI_XSDT* xsdt, const char* name) {
    ACPI_SDT_HEADER* header;
    size_t entries = (xsdt->header.len - sizeof(ACPI_SDT_HEADER)) / 8;
    
    for (size_t i = 0; i < entries; i++) {
        header = (ACPI_SDT_HEADER*)xsdt->entry[i];

        if (!strncmp(header->sign, name, 4)) {
            return (void*)header;
        }
    }

    return NULL;
}