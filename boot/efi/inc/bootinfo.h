#include <stdint.h>
#include <stddef.h>


typedef struct {
    uint32_t            type;
    uint32_t            pad;
    uint64_t            phys_addr;
    uint64_t            vir_addr;
    uint64_t            num_pages;
    uint64_t            attr;
}__attribute__((packed)) MEMORY_DESCRIPTOR;
typedef struct {
    uint64_t framebuffer_base;
    size_t framebuffer_size;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
}__attribute__((packed)) GRAPHICS_OUTPUT_PROTOCOL;
typedef struct {
    MEMORY_DESCRIPTOR* map;
    size_t size;
    size_t desc_size;
}__attribute__((packed)) MEMORY_MAP;
typedef struct {
    GRAPHICS_OUTPUT_PROTOCOL gop;
    MEMORY_MAP mem_map;
    uint64_t xsdt_addr;
}__attribute__((packed)) BOOTINFO;