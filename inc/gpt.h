#ifndef _GPT_H
#define _GPT_H

#include <stdint.h>


typedef struct {
    uint8_t sign[8];
    uint32_t rev;
    uint32_t header_size;
    uint32_t header_chksum;
    uint32_t reserved;
    uint64_t header_lba;
    uint64_t backup_header_lba;
    uint64_t part_start_lba;
    uint64_t part_end_lba;
    uint8_t disk_guid[16];
    uint64_t part_tbl_start_lba;
    uint32_t part_entry_num;
    uint32_t gpt_entry_size;
    uint32_t part_tbl_chksum;
}__attribute__((packed)) GPT_HEADER;
typedef struct {
    uint8_t part_type_guid[16];
    uint8_t part_guid[16];
    uint64_t part_start_lba;
    uint64_t part_end_lba;
    uint64_t part_attr;
    uint16_t part_name[36];
}__attribute__((packed)) GPT_PARTITION_ENTRY;

#endif