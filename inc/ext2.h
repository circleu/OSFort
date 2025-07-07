#ifndef _EXT2_H
#define _EXT2_H

#include <stdint.h>


typedef struct {
    uint32_t inode_cnt;
    uint32_t block_cnt;
    uint32_t r_block_cnt;
    uint32_t free_block_cnt;
    uint32_t free_inode_cnt;
    uint32_t first_data_block;
    uint32_t log_block_size; // log_2(block size)
    uint32_t log_frag_size; // log_2(frag size)
    uint32_t block_per_group;
    uint32_t frag_per_group;
    uint32_t inode_per_group;
    uint32_t mtime; // POSIX time
    uint32_t wtime; // POSIX time
    uint16_t mnt_cnt;
    uint16_t max_mnt_cnt;
    uint16_t magic;
    uint16_t state;
    uint16_t error;
    uint16_t minor_rev_level;
    uint32_t last_chk; // POSIX time
    uint32_t chk_interval; // POSIX time
    uint32_t creator_os;
    uint16_t rev_level;
    uint16_t def_resuid;
    uint16_t def_resgid;
    uint32_t first_inode;
    uint16_t inode_size;
    uint16_t block_group_nr;
    uint32_t feature_compat;
    uint32_t feature_incompat;
    uint32_t feature_ro_compat;
    uint8_t uuid[16];
    uint8_t vol_name[16];
    uint8_t last_mnt[64];
    uint32_t algo_bitmap;
    
}__attribute__((packed)) EXT2_SUPERBLOCK;



#endif