#ifndef _KERNEL_PAGING_H
#define _KERNEL_PAGING_H

#include "../../sys/stdint.h"
#include "../../sys/string.h"
#include "../../inc/asm.h"
#include "bitmap.h"

#define PAGE_SIZE 0x1000
/* From kernel.h */
#define KERNEL_START 0x0000FFFF00000000
#define phys_to_vir(phys_addr) ((phys_addr) - (size_t)&_kernel_start + KERNEL_START)

typedef struct {
    BITMAP bitmap;
    size_t free;
    size_t reserved;
    size_t used;
}__attribute__((packed)) PAGE_BITMAP;
/* use pml4, so 52-bit phys, 48-bit vir */
typedef struct {
    uint8_t p : 1;
    uint8_t rw : 1;
    uint8_t us : 1;
    uint8_t pwt : 1;
    uint8_t pcd : 1;
    uint8_t a : 1;
    uint8_t avl0 : 1;
    uint8_t ps : 1;
    uint8_t avl1 : 4;
    uint64_t addr : 40;
    uint16_t avl2 : 11;
    uint8_t xd : 1;
}__attribute__((packed)) PAGE_TABLE_ENTRY;
typedef struct {
    PAGE_TABLE_ENTRY entries[512];
}__attribute__((aligned(0x1000))) PAGE_TABLE;

void free_page(PAGE_BITMAP* page_bitmap, void* addr);
void free_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt);
void lock_page(PAGE_BITMAP* page_bitmap, void* addr);
void lock_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt);
void unreserve_page(PAGE_BITMAP* page_bitmap, void* addr);
void unreserve_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt);
void reserve_page(PAGE_BITMAP* page_bitmap, void* addr);
void reserve_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt);
void* req_page(PAGE_BITMAP* page_bitmap);
void map_page(PAGE_BITMAP* page_bitmap, PAGE_TABLE* pml4, void* vir_addr, void* phys_addr);
void map_pages(PAGE_BITMAP* page_bitmap, PAGE_TABLE* pml4, void* vir_addr, void* phys_addr, size_t page_cnt);
uint64_t vir_to_phys(uint64_t vir_addr);

extern size_t _kernel_start;


#endif