#ifndef _DEV_AHCI_H
#define _DEV_AHCI_H

#include "../../sys/stdint.h"
#include "../../sys/stddef.h"


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
    uint8_t base_addr[20];
    uint32_t ahci_base_addr;
    uint32_t res0;
    uint16_t subsys_vendor_id;
    uint16_t subsys_id;
    uint32_t expansion_rom_base_addr;
    uint8_t capabilities_ptr;
    uint8_t res1[7];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_gnt;
    uint8_t max_lat;
}__attribute__((packed)) AHCI_PCI_HEADER;


#endif