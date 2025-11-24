#include "kernel.h"


MEMORY_MAP efi_mmap;
ACPI_XSDT* xsdt;
ACPI_MADT* madt;
ACPI_MCFG* mcfg;
uint32_t* apic_base;
PAGE_TABLE* pml4;
BOOTDISK_ADDRESS bootdisk_addr;

extern void _start(void* BootInfo) {
    graphics = (GRAPHICS){
        ((BOOTINFO*)BootInfo)->gop.framebuffer_base,
        ((BOOTINFO*)BootInfo)->gop.framebuffer_size,
        ((BOOTINFO*)BootInfo)->gop.width,
        ((BOOTINFO*)BootInfo)->gop.height,
        ((BOOTINFO*)BootInfo)->gop.pixels_per_scanline,
        0,
        0
    };
    efi_mmap = ((BOOTINFO*)BootInfo)->mem_map;
    xsdt = (ACPI_XSDT*)(((BOOTINFO*)BootInfo)->xsdt_addr);
    bootdisk_addr = ((BOOTINFO*)BootInfo)->bootdisk_addr;

/*******************************************************************/

    kprintf("Loading GDT...");
    {
        load_gdt(&gdtr);
    }
    kprintf("Done!\n");
    
    kprintf("Setting Paging...");
    {
        size_t mmap_entries = efi_mmap.size / efi_mmap.desc_size;
        void* largest_desc = NULL;
        size_t largest_desc_size = 0;
        size_t total_mem_size = 0;

        for (size_t i = 0; i < mmap_entries; i++) {
            MEMORY_DESCRIPTOR* desc = (MEMORY_DESCRIPTOR*)((uint8_t*)efi_mmap.map + efi_mmap.desc_size * i);
            total_mem_size += desc->num_pages * PAGE_SIZE;

            /* EfiConventionalMemory */
            if (desc->type == 7) {
                if (desc->num_pages * PAGE_SIZE > largest_desc_size) {
                    largest_desc = (void*)desc->phys_addr;
                    largest_desc_size = desc->num_pages * PAGE_SIZE;
                }
            }
        }

        page_bitmap.free = total_mem_size;
        page_bitmap.bitmap.size = total_mem_size / PAGE_SIZE / 8 + 1;
        page_bitmap.bitmap.buffer = (uint8_t*)largest_desc;
        memset(page_bitmap.bitmap.buffer, 0, page_bitmap.bitmap.size);
        lock_pages(&page_bitmap, (void*)page_bitmap.bitmap.buffer, page_bitmap.bitmap.size / PAGE_SIZE + 1);

        for (size_t i = 0; i < mmap_entries; i++) {
            MEMORY_DESCRIPTOR* desc = (MEMORY_DESCRIPTOR*)((uint8_t*)efi_mmap.map + efi_mmap.desc_size * i);

            if (desc->type != 7) {
                reserve_pages(&page_bitmap, (void*)desc->phys_addr, desc->num_pages);
            }
        }

        size_t kernel_size = (size_t)&_kernel_end - (size_t)&_kernel_start;
        size_t kernel_pages = kernel_size / PAGE_SIZE + 1;
        reserve_pages(&page_bitmap, &_kernel_start, kernel_pages);

        pml4 = (PAGE_TABLE*)req_page(&page_bitmap);
        memset(pml4, 0, PAGE_SIZE);

        map_pages(&page_bitmap, pml4, (void*)0, (void*)0, total_mem_size / PAGE_SIZE);

        lock_pages(&page_bitmap, (void*)graphics.framebuffer_base, graphics.framebuffer_size / PAGE_SIZE + 1);
        map_pages(&page_bitmap, pml4, (void*)graphics.framebuffer_base, (void*)graphics.framebuffer_base, graphics.framebuffer_size / PAGE_SIZE + 1);

        map_pages(&page_bitmap, pml4, (void*)(KERNEL_START + (size_t)&_kernel_start), (void*)&_kernel_start, kernel_pages);

        madt = acpi_get_table(xsdt, "APIC");
        apic_base = (uint32_t*)((uint64_t)madt->local_apic_addr);
        map_page(&page_bitmap, pml4, (void*)apic_base, (void*)apic_base);

        mcfg = acpi_get_table(xsdt, "MCFG");
        size_t mcfg_pages = mcfg->header.len / PAGE_SIZE + 1;
        map_pages(&page_bitmap, pml4, (void*)mcfg, (void*)mcfg, mcfg_pages);
        
        __asm__ volatile (
            "movq %0, %%cr3;"
            "leaq 0f, %%rax;"
            "jmp *%%rax;"
            "0:"
            ::"r"(pml4)
        );
    }
    kprintf("Done!\n");

    kprintf("Setting IDT...");
    {
        set_idt();
    }
    kprintf("Done!\n");

    kprintf("Setting APIC Timer...");
    {
        acpi_set_apic_timer(madt, 100);
    }
    kprintf("Done!\n");

    // For now, I'm gonna implement NVMe only.
    kprintf("Finding Bootdisk and Initialising NVMe...\n");
    {
        uint64_t bootdisk_config_space_addr = (
            mcfg->config_space[bootdisk_addr.seg].base_addr + 
            ((size_t)bootdisk_addr.bus << 20) + 
            ((size_t)bootdisk_addr.dev << 15) + 
            ((size_t)bootdisk_addr.func << 12)
        );
        uint32_t class_code = *(uint32_t*)(bootdisk_config_space_addr + 0x08);
        PCIE_CONFIGURATION_SPACE_HEADER_TYPE_0 bootdisk_config = {0, };

        if (
            ((class_code >> 24) == 0x01) &&
            (((class_code >> 16) & 0xff) == 0x08)
        ) {
            kprintf("\tFound Bootdisk.\n");
        }
        else {
            kprintf("\tCould not find bootdisk.\n");
            while(1);
        }

        {
            uint32_t tmp[PCIE_CONFIGURATION_SPACE_HEADER_SIZE / 4] = {0, };

            for (size_t i = 0; i < PCIE_CONFIGURATION_SPACE_HEADER_SIZE / 4; i++) {
                tmp[i] = ((volatile uint32_t*)bootdisk_config_space_addr)[i];
            }

            memcpy((void*)&bootdisk_config, (void*)tmp, PCIE_CONFIGURATION_SPACE_HEADER_SIZE);
        }

        uint64_t bootdisk_cntl_addr = (uint64_t)(bootdisk_config.base_addr0 & 0xfffffff0) | ((uint64_t)bootdisk_config.base_addr1 << 32);
        map_pages(&page_bitmap, pml4, (void*)bootdisk_cntl_addr, (void*)bootdisk_cntl_addr, 3);

        kprintf("\tDisabling NVMe Controller...");
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 0, 0, 1);
        while (nvme_mmio_read_32(bootdisk_cntl_addr, NVME_CSTS_OFFSET, 0, 1));
        kprintf("Done!\n");

        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_AQA_OFFSET, 0, NVME_ADMIN_QUEUE_ENTRIES - 1, 12);
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_AQA_OFFSET, 16, NVME_ADMIN_QUEUE_ENTRIES - 1, 12);
        nvme_mmio_write_64(bootdisk_cntl_addr, NVME_ASQ_OFFSET, 12, vir_to_phys((uint64_t)&asq), 52);
        nvme_mmio_write_64(bootdisk_cntl_addr, NVME_ACQ_OFFSET, 12, vir_to_phys((uint64_t)&acq), 52);
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 4, 0, 3);
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 7, 0, 4);
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 11, 0, 3);
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 14, 0, 2);
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 16, NVME_SQE_SIZE_LOG, 4);
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 20, NVME_CQE_SIZE_LOG, 4);

        kprintf("\tEnabling NVMe Controller...");
        nvme_mmio_write_32(bootdisk_cntl_addr, NVME_CC_OFFSET, 0, 1, 1);
        while (!(nvme_mmio_read_32(bootdisk_cntl_addr, NVME_CSTS_OFFSET, 0, 1)));
        kprintf("Done!\n");

        // nvme_command_identify(bootdisk_cntl_addr);

        // kprintf("%s\n", identify_buf.mn);

        
    }
    kprintf("Done!\n");

    kprintf("Setting syscall...");
    {
/* set IA32_EFER.SCE (bit 0) to enable syscall/sysret instruction */
        wrmsr(0xc0000080, rdmsr(0xc0000080) | 1);
        
/* write syscall handler address to IA32_LSTAR */
        wrmsr(0xc0000082, (uint64_t)syscall_handler);
            
/* write segment selectors to IA32_STAR */
/* kernel code for syscall, user code for sysret */
        wrmsr(0xc0000081, (rdmsr(0xc0000081) & 0xffffffff) | ((uint64_t)0x08 << 32) | ((uint64_t)0x18 << 48));
        wrmsr(0xc0000084, rdmsr(0xc0000084) | (uint64_t)(1 << 9));
    }
    kprintf("Done!\n");

    kprintf("Entering user mode...\n\n");
    

    
    
    while (1);
}