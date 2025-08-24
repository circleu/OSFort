#include <stdint.h>


typedef struct {
    uint8_t sign[4];
    uint32_t len;
    uint8_t rev;
    uint8_t chksum;
    uint8_t oem_id[6];
    uint8_t oem_table_id[8];
    uint32_t oem_rev;
    uint32_t creator_id;
    uint32_t creator_rev;   
}__attribute__((packed)) ACPI_SDT_HEADER;
typedef struct {
    uint8_t sign[8];
    uint8_t chksum;
    uint8_t oemid[6];
    uint8_t rev;
    uint32_t rsdt_addr;

    uint32_t len;
    uint64_t xsdt_addr;
    uint8_t ext_chksum;
    uint8_t res[3];
}__attribute__((packed)) ACPI_XSDP;
typedef struct {
    ACPI_SDT_HEADER header;
    uint64_t* entry;
}__attribute__((packed)) ACPI_XSDT;