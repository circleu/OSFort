#ifndef _DEV_PCIE_H
#define _DEV_PCIE_H

#define PCIE_DEVICE_NUM 32
#define PCIE_FUNCTION_NUM 8
#define PCIE_CONFIGURATION_SPACE_HEADER_SIZE 64
#define PCIE_CONFIGURATION_SPACE_SIZE 4096

#include "../../sys/stdint.h"
#include "../../inc/asm.h"
#include "../../dev/acpi/acpi.h"
#include "../../sys/stdarg.h"



typedef struct {
    uint8_t mem_io : 1;         // always 0
    uint8_t mem_type : 2;
    uint8_t prefetchable : 1;
    uint32_t base_addr : 28;
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_BASE_ADDRESS_REGISTER_MEMORY;
typedef struct {
    uint8_t mem_io : 1;         // always 1
    uint8_t res : 1;            // always 0
    uint32_t base_addr : 30;
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_BASE_ADDRESS_REGISTER_IO;
typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t rev_id;
    uint32_t class_code : 24;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint8_t type_specific0[36];
    uint8_t capabilities_ptr;
    uint8_t type_specific1[7];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t type_specific2[2];
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_HEADER;
typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t rev_id;
    uint32_t class_code : 24;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint32_t base_addr0;
    uint32_t base_addr1;
    uint32_t base_addr2;
    uint32_t base_addr3;
    uint32_t base_addr4;
    uint32_t base_addr5;
    uint32_t card_bus_ptr;
    uint16_t subsys_vendor_id;
    uint16_t subsys_id;
    uint32_t expansion_rom_base_addr;
    uint8_t capabilities_ptr;
    uint8_t res[7];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_gnt;
    uint8_t max_lat;
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_HEADER_TYPE_0;
typedef struct {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t rev_id;
    uint32_t class_code : 24;
    uint8_t cache_line_size;
    uint8_t primary_latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint32_t base_addr0;
    uint32_t base_addr1;
    uint8_t primary_bus_num;
    uint8_t secondary_bus_num;
    uint8_t subordinate_bus_num;
    uint8_t secondary_latency_num;
    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;
    uint16_t memory_base;
    uint16_t memory_limit;
    uint16_t prefetchable_memory_base;
    uint16_t prefetchable_memory_limit;
    uint32_t prefetchable_memory_base_upper_32_bits;
    uint32_t prefetchable_memory_limit_upper_32_bits;
    uint16_t io_base_upper_16_bits;
    uint16_t io_limit_upper_16_bits;
    uint8_t capabilities_ptr;
    uint8_t res[3];
    uint32_t expansion_rom_base_addr;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_cntl;
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_HEADER_TYPE_1;

uint8_t* pcie_concat_split_data(size_t argc, ...);
uint64_t* pcie_get_config_space_addrs(ACPI_MCFG* mcfg);
PCIE_CONFIGURATION_SPACE_HEADER pcie_read_config_space_header(uint64_t config_space_addr);


#endif