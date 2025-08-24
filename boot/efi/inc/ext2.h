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
    uint32_t rev_level;
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
    uint8_t prealloc_block;
    uint8_t prealloc_dir_block;
    uint8_t align[2];
    uint8_t journal_uuid[16];
    uint32_t journal_inode_num;
    uint32_t journal_dev;
    uint32_t last_orphan;
    uint32_t hash_seed[4];
    uint8_t def_hash_ver;
    uint8_t res0[3];
    uint32_t def_mnt_opt;
    uint32_t first_meta_bg;
    uint8_t res1[760];
}__attribute__((packed)) EXT2_SUPERBLOCK;
typedef struct {
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t inode_table;
    uint16_t free_block_cnt;
    uint16_t free_inode_cnt;
    uint16_t used_dir_cnt;
    uint16_t align;
    uint8_t res0[12];
}__attribute__((packed)) EXT2_BLOCK_GROUP_DESCRIPTOR;
typedef struct {
    uint16_t mode;
    uint16_t uid;
    uint32_t size;
    uint32_t atime;
    uint32_t ctime;
    uint32_t mtime;
    uint32_t dtime;
    uint16_t gid;
    uint16_t link_cnt;
    uint32_t block_cnt; // "BLOCK" IS NOT THE BLOCK OF FILESYSTEM!!!. "BLOCK" MEANS 512 BYTES IN HERE!!!
    uint32_t flag;
    uint32_t osd1;
    uint32_t block[15];
    uint32_t gen;
    uint32_t file_acl;
    uint32_t dir_acl;
    uint32_t faddr;
    uint32_t osd2[3];
}__attribute__((packed)) EXT2_INODE;
typedef struct {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    uint8_t name[255]; // 0 - 255
}__attribute__((packed)) EXT2_DIRECTORY_ENTRY;