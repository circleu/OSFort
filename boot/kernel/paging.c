#include "paging.h"


PAGE_BITMAP page_bitmap = {0, };

void free_page(PAGE_BITMAP* page_bitmap, void* addr) {
    size_t i = (size_t)addr / 0x1000;

    if (get_bitmap(&(page_bitmap->bitmap), i)) {
        set_bitmap(&(page_bitmap->bitmap), i, 0);

        page_bitmap->free += 0x1000;
        page_bitmap->used -= 0x1000;
    }
    else return;
}
void free_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt) {
    for (size_t i = 0; i < page_cnt; i++)
        free_page(page_bitmap, (void*)((uint8_t*)addr + i * 0x1000));
}
void lock_page(PAGE_BITMAP* page_bitmap, void* addr) {
    size_t i = (size_t)addr / 0x1000;

    if (!get_bitmap(&(page_bitmap->bitmap), i)) {
        set_bitmap(&(page_bitmap->bitmap), i, 1);

        page_bitmap->free -= 0x1000;
        page_bitmap->used += 0x1000;
    }
    else return;
}
void lock_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt) {
    for (size_t i = 0; i < page_cnt; i++)
        lock_page(page_bitmap, (void*)((uint8_t*)addr + i * 0x1000));
}
void unreserve_page(PAGE_BITMAP* page_bitmap, void* addr) {
    size_t i = (size_t)addr / 0x1000;

    if (get_bitmap(&(page_bitmap->bitmap), i)) {
        set_bitmap(&(page_bitmap->bitmap), i, 0);

        page_bitmap->free += 0x1000;
        page_bitmap->reserved -= 0x1000;
    }
    else return;
}
void unreserve_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt) {
    for (size_t i = 0; i < page_cnt; i++)
        unreserve_page(page_bitmap, (void*)((uint8_t*)addr + i * 0x1000));
}
void reserve_page(PAGE_BITMAP* page_bitmap, void* addr) {
    size_t i = (size_t)addr / 0x1000;

    if (!get_bitmap(&(page_bitmap->bitmap), i)) {
        set_bitmap(&(page_bitmap->bitmap), i, 1);

        page_bitmap->free -= 0x1000;
        page_bitmap->reserved += 0x1000;
    }
    else return;
}
void reserve_pages(PAGE_BITMAP* page_bitmap, void* addr, size_t page_cnt) {
    for (size_t i = 0; i < page_cnt; i++)
        reserve_page(page_bitmap, (void*)((uint8_t*)addr + i * 0x1000));
}
void* req_page(PAGE_BITMAP* page_bitmap) {
    for (size_t i = 0; i < page_bitmap->bitmap.size * 8; i++) {
        if (get_bitmap(&(page_bitmap->bitmap), i)) continue;
        else {
            lock_page(page_bitmap, (void*)(i * 0x1000));

            return (void*)(i * 0x1000);
        }
    }
    return NULL;
}
void map_page(PAGE_BITMAP* page_bitmap, PAGE_TABLE* pml4, void* vir_addr, void* phys_addr) {
    uint64_t p_i = ((uint64_t)vir_addr >> 12) & 0x1ff;
    uint64_t pt_i = ((uint64_t)vir_addr >> 21) & 0x1ff;
    uint64_t pd_i = ((uint64_t)vir_addr >> 30) & 0x1ff;
    uint64_t pdpt_i = ((uint64_t)vir_addr >> 39) & 0x1ff;
    PAGE_TABLE_ENTRY pe;

    PAGE_TABLE* pdpt;
    pe = pml4->entries[pdpt_i];
    if (!pe.p) {
        pdpt = (PAGE_TABLE*)req_page(page_bitmap);
        memset(pdpt, 0, 0x1000);

        pe.addr = (uint64_t)pdpt >> 12;
        pe.p = 1;
        pe.rw = 1;
        pml4->entries[pdpt_i] = pe;
    }
    else {
        pdpt = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }

    PAGE_TABLE* pd;
    pe = pdpt->entries[pd_i];
    if (!pe.p) {
        pd = (PAGE_TABLE*)req_page(page_bitmap);
        memset(pd, 0, 0x1000);

        pe.addr = (uint64_t)pd >> 12;
        pe.p = 1;
        pe.rw = 1;
        pdpt->entries[pd_i] = pe;
    }
    else {
        pd = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }

    PAGE_TABLE* pt;
    pe = pd->entries[pt_i];
    if (!pe.p) {
        pt = (PAGE_TABLE*)req_page(page_bitmap);
        memset(pt, 0, 0x1000);

        pe.addr = (uint64_t)pt >> 12;
        pe.p = 1;
        pe.rw = 1;
        pd->entries[pt_i] = pe;
    }
    else {
        pt = (PAGE_TABLE*)((uint64_t)pe.addr << 12);
    }

    pe = pt->entries[p_i];
    pe.addr = (uint64_t)phys_addr >> 12;
    pe.p = 1;
    pe.rw = 1;
    pt->entries[p_i] = pe;
}