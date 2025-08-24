#ifndef _DEV_PCIE_H
#define _DEV_PCIE_H

#include "../../sys/stdint.h"
#include "../../inc/asm.h"
#include "../../dev/acpi/acpi.h"
#include "../../sys/stdarg.h"


typedef struct {
    uint8_t type : 5;
    uint8_t fmt : 3;
    uint8_t th : 1;
    uint8_t ln : 1;
    uint8_t attr0 : 1;
    uint8_t t8 : 1;
    uint8_t tc : 3;
    uint8_t t9 : 1;
    uint8_t length0 : 2;
    uint8_t at : 2;
    uint8_t attr1 : 2;
    uint8_t ep : 1;
    uint8_t td : 1;
    uint8_t length1;
}__attribute__((packed)) TLP_HEADER;
typedef struct {
    uint8_t bus;
    uint8_t func : 3;
    uint8_t dev : 5;
}__attribute__((packed)) TLP_NON_ARI_ID_ROUTING_HEADER;
typedef struct {
    uint8_t bus;
    uint8_t func;
}__attribute__((packed)) TLP_ARI_ID_ROUTING_HEADER;
/*
typedef struct {
    TLP_HEADER tlp_header;
    4...7
    uint64_t addr0 : 56;
    uint8_t ph : 2;
    uint8_t addr1 : 6;
}__attribute__((packed)) TLP_ADDRESS_ROUTING_64;
*/
/*
typedef struct {
    TLP_HEADER tlp_header;
    4...7
    uint32_t addr0 : 24;
    uint8_t ph : 2;
    uint8_t addr1 : 6;
}__attribute__((packed)) TLP_ADDRESS_ROUTING_32;
*/
/*
typedef struct {
    TLP_HEADER tlp_header;
    4...7
    TLP_NON_ARI_ID_ROUTING_HEADER tlp_id_routing_header;
    10...11
    12...15
}__attribute__((packed)) TLP_NON_ARI_ID_ROUTING_4_DW;
*/
/*
typedef struct {
    TLP_HEADER tlp_header;
    4...7
    TLP_ARI_ID_ROUTING_HEADER tlp_id_routing_header;
    10...11
    12...15
}__attribute__((packed)) TLP_ARI_ID_ROUTING_4_DW;
*/
/*
typedef struct {
    TLP_HEADER tlp_header;
    4...7
    TLP_NON_ARI_ID_ROUTING_HEADER tlp_id_routing_header;
    10...11
}__attribute__((packed)) TLP_NON_ARI_ID_ROUTING_3_DW;
*/
/*
typedef struct {
    TLP_HEADER tlp_header;
    4...7
    TLP_ARI_ID_ROUTING_HEADER tlp_id_routing_header;
    10...11
}__attribute__((packed)) TLP_ARI_ID_ROUTING_3_DW;
*/
/*
typedef struct {
    TLP_HEADER tlp_header;
    4...6
    uint8_t first_dw_be : 4;
    uint8_t last_dw_be : 4;
}__attribute__((packed)) TLP_BYTE_ENABLES;
*/
typedef struct {
    TLP_HEADER tlp_header;
    uint8_t req_id[2];
    uint8_t tag;
    uint8_t first_dw_be : 4;
    uint8_t last_dw_be : 4;
    uint64_t addr0 : 56;
    uint8_t ph : 2;
    uint8_t addr1 : 6;
}__attribute__((packed)) TLP_MEMORY_REQUEST_64;
typedef struct {
    TLP_HEADER tlp_header;
    uint8_t req_id[2];
    uint8_t tag;
    uint8_t first_dw_be : 4;
    uint8_t last_dw_be : 4;
    uint32_t addr0 : 24;
    uint8_t ph : 2;
    uint8_t addr1 : 6;
}__attribute__((packed)) TLP_MEMORY_REQUEST_32;
typedef struct {
    TLP_HEADER tlp_header;
    uint8_t req_id[2];
    uint8_t tag;
    uint8_t first_dw_be : 4;
    uint8_t last_dw_be : 4;
    uint32_t addr0 : 24;
    uint8_t res : 2;
    uint8_t addr1 : 6;
}__attribute__((packed)) TLP_IO_REQUEST;
typedef struct {
    TLP_HEADER tlp_header;
    uint8_t req_id[2];
    uint8_t tag;
    uint8_t first_dw_be : 4;
    uint8_t last_dw_be : 4;
    TLP_NON_ARI_ID_ROUTING_HEADER tlp_id_routing_header;
    uint8_t ext_reg_num : 4;
    uint8_t res0 : 4;
    uint8_t res1 : 2;
    uint8_t reg_num : 6;
}__attribute__((packed)) TLP_CONFIGURATION_REQUEST;
typedef struct {
    uint8_t type : 5;
    uint8_t fmt : 3;
    uint8_t st;
    uint8_t res[2];
}__attribute__((packed)) TPH_TLP_PREFIX;
typedef struct {
    uint8_t req_id[2];
    uint8_t st;
    uint8_t first_dw_be : 4;
    uint8_t last_dw_be : 4;
}__attribute__((packed)) TLP_MEMORY_WRITE_REQUEST_HEADER;
typedef struct {
    TLP_HEADER tlp_header;
    uint8_t req_id[2];
    uint8_t tag;
    uint8_t st;
}__attribute__((packed)) TLP_MEMORY_READ_ATOMIC_REQUEST_HEADER;
typedef struct {
    TLP_HEADER tlp_header;
    uint8_t req_id[2];
    uint8_t tag;
    uint8_t message;
}__attribute__((packed)) TLP_MESSAGE_REQUEST_HEADER;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t res0 : 6;
    uint8_t ecs : 2;
    uint8_t res1[7];
}__attribute__((packed)) TLP_ERR_COR_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    TLP_NON_ARI_ID_ROUTING_HEADER tlp_id_routing_header;
    uint8_t vendor_id[2];
    uint8_t vendor_def[4];
}__attribute__((packed)) TLP_VENDOR_DEFINED_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t dest_id[2];
    uint8_t vendor_id[2];
    uint8_t sybtype;
    uint8_t pci_sig_def_vdm_def[3];
}__attribute__((packed)) TLP_PCI_SIG_DEFINED_VDM;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t dest_id[2];
    uint8_t vendor_id[2];
    uint8_t sybtype;
    uint8_t res0[3];
    uint8_t cacheline_addr0[7];
    uint8_t nr : 2;
    uint8_t res1 : 4;
    uint8_t cacheline_addr1 : 2;
}__attribute__((packed)) TLP_LN_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t res0[2];
    uint8_t vendor_id[2];
    uint8_t subtype;
    uint8_t res1[3];
}__attribute__((packed)) TLP_DRS_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t res0[2];
    uint8_t vendor_id[2];
    uint8_t subtype;
    uint8_t res1[2];
    uint8_t frs_reason : 4;
    uint8_t res2 : 4;
}__attribute__((packed)) TLP_FRS_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t hierarchy_id[2];
    uint8_t vendor_id[2];
    uint8_t subtype;
    uint8_t guid_auth_id;
    uint8_t sys_guid[18];
}__attribute__((packed)) TLP_HIERARCHY_ID_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t res[4];
    uint8_t no_snoop_latency[2];
    uint8_t snoop_latency[2];
}__attribute__((packed)) TLP_LTR_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t res0[7];
    uint8_t obff_code : 4;
    uint8_t res1 : 4;
}__attribute__((packed)) TLP_OBFF_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t res[8];
}__attribute__((packed)) TLP_PTM_REQUEST_RESPONSE_MESSAGE;
typedef struct {
    TLP_MESSAGE_REQUEST_HEADER tlp_msg_req_header;
    uint8_t ptm_master_time[12];
}__attribute__((packed)) TLP_PTM_RESPONSED_MESSAGE;
typedef struct {
    TLP_HEADER tlp_header;
    uint8_t completer_id[2];
    uint8_t byte_cnt0 : 4;
    uint8_t bcm : 1;
    uint8_t cpl_status : 3;
    uint8_t byte_cnt1;
}__attribute__((packed)) TLP_COMPLETION_HEADER;
/*
typedef struct {
    uint8_t vendor_id[2];
    uint8_t device_id[2];
    uint8_t command[2];
    uint8_t status[2];
    uint8_t rev_id;
    uint8_t class_code[3];
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    16...51
    uint8_t capabilities_ptr;
    53...59
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    62...63
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_HEADER;
*/
typedef struct {
    uint8_t vendor_id[2];
    uint8_t device_id[2];
    uint8_t command[2];
    uint8_t status[2];
    uint8_t rev_id;
    uint8_t class_code[3];
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint8_t base_addr[24];
    uint8_t card_bus_ptr[4];
    uint8_t subsys_vendor_id[2];
    uint8_t subsys_id[2];
    uint8_t expansion_rom_base_addr[4];
    uint8_t capabilities_ptr;
    uint8_t res[7];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t min_gnt;
    uint8_t max_lat;
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_HEADER_TYPE_0;
typedef struct {
    uint8_t vendor_id[2];
    uint8_t device_id[2];
    uint8_t command[2];
    uint8_t status[2];
    uint8_t rev_id;
    uint8_t class_code[3];
    uint8_t cache_line_size;
    uint8_t primary_latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint8_t base_addr0[4];
    uint8_t base_addr1[4];
    uint8_t primary_bus_num;
    uint8_t secondary_bus_num;
    uint8_t subordinate_bus_num;
    uint8_t secondary_latency_num;
    uint8_t io_base;
    uint8_t io_limit;
    uint8_t secondary_status[2];
    uint8_t memory_base[2];
    uint8_t memory_limit[2];
    uint8_t prefetchable_memory_base[2];
    uint8_t prefetchable_memory_limit[2];
    uint8_t prefetchable_memory_base_upper_32_bits[4];
    uint8_t prefetchable_memory_limit_upper_32_bits[4];
    uint8_t io_base_upper_16_bits[2];
    uint8_t io_limit_upper_16_bits[2];
    uint8_t capabilities_ptr;
    uint8_t res[3];
    uint8_t expansion_rom_base_addr[4];
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t bridge_cntl[2];
}__attribute__((packed)) PCIE_CONFIGURATION_SPACE_HEADER_TYPE_1;

size_t pcie_array_to_uint64(uint8_t ptr[], size_t size);


#endif