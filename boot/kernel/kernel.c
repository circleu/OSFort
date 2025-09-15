#include "kernel.h"

#define KERNEL_START 0xC0000000


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
    MEMORY_MAP efi_mmap = ((BOOTINFO*)BootInfo)->mem_map;
    ACPI_XSDT* xsdt = (ACPI_XSDT*)(((BOOTINFO*)BootInfo)->xsdt_addr);
    ACPI_MADT* madt;
    ACPI_MCFG* mcfg;
    uint32_t* apic_base;
    PAGE_TABLE* pml4;
    BOOTDISK_ADDRESS bootdisk_addr = ((BOOTINFO*)BootInfo)->bootdisk_addr;

/*******************************************************************/

    kputs("Loading GDT...");
    {
        load_gdt(&gdtr);
    }
    kputs("Done!\r\n");
    
    kputs("Setting Paging...");
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

        map_pages(&page_bitmap, pml4, (void*)KERNEL_START, (void*)_kernel_start, kernel_pages);

        madt = acpi_get_table(xsdt, "APIC");
        apic_base = (uint32_t*)((uint64_t)madt->local_apic_addr);
        map_page(&page_bitmap, pml4, (void*)apic_base, (void*)apic_base);

        mcfg = acpi_get_table(xsdt, "MCFG");
        size_t mcfg_pages = mcfg->header.len / PAGE_SIZE + 1;
        map_pages(&page_bitmap, pml4, (void*)mcfg, (void*)mcfg, mcfg_pages);
        
        __asm__ volatile ("movq %0, %%cr3"::"r"(pml4));
    }
    kputs("Done!\r\n");

    kputs("Setting IDT...");
    {
        set_idt();
    }
    kputs("Done!\r\n");

    kputs("Setting APIC Timer...");
    {
        acpi_set_apic_timer(madt, 100);
    }
    kputs("Done!\r\n");

    kputs("Finding Bootdisk and Initialising NVMe...\r\n");
    {
        uint64_t bootdisk_config_space_addr = (
            mcfg->config_space[bootdisk_addr.seg].base_addr + 
            ((size_t)bootdisk_addr.bus << 20) + 
            ((size_t)bootdisk_addr.dev << 15) + 
            ((size_t)bootdisk_addr.func << 12)
        );
        uint32_t class_code = *(uint32_t*)(bootdisk_config_space_addr + 0x08);
        PCIE_CONFIGURATION_SPACE_HEADER_TYPE_0 bootdisk_config;

        if (
            (class_code >> 24) == 0x01 &&
            ((class_code >> 16) & 0xff) == 0x08
        ) {
            kputs("\tFound Bootdisk.\r\n");
        }
        else {
            kputs("\tCould not find bootdisk.\r\n");
            while(1);
        }

        for (size_t i = 0; i < PCIE_CONFIGURATION_SPACE_HEADER_SIZE / 4; i++) {
            ((uint32_t*)&bootdisk_config)[i] = ((uint32_t*)bootdisk_config_space_addr)[i];
        }
        /*

        uint64_t bootdisk_cntl_addr = ((uint64_t)bootdisk_config.base_addr0 & 0xfffffff0) | ((uint64_t)bootdisk_config.base_addr1 << 32);
        map_page(&page_bitmap, pml4, (void*)bootdisk_cntl_addr, (void*)bootdisk_cntl_addr);
        invlpg(bootdisk_cntl_addr);
        
        NVME_CONTROLLER_REGISTERS* bootdisk_cntl = (NVME_CONTROLLER_REGISTERS*)((uint64_t)bootdisk_cntl_addr);

        bootdisk_cntl->aqa.asqs = (uint16_t)NVME_ADMIN_QUEUE_ENTRIES - 1;
        bootdisk_cntl->aqa.acqs = (uint16_t)NVME_ADMIN_QUEUE_ENTRIES - 1;
        bootdisk_cntl->asq.asqb = (uint64_t)&asq & 0x000FFFFFFFFFFFFF;
        bootdisk_cntl->acq.acqb = (uint64_t)&acq & 0x000FFFFFFFFFFFFF;
        bootdisk_cntl->cc.iosqes = (uint8_t)NVME_SQE_SIZE_LOG;
        bootdisk_cntl->cc.iocqes = (uint8_t)NVME_CQE_SIZE_LOG;
        bootdisk_cntl->cc.en = (uint8_t)1;

        kputs("\tWaiting for NVMe Controller...");
        while (!(bootdisk_cntl->csts.rdy));
        kputs("Done!\r\n");
        
        */
    }
    kputs("Done!\r\n");

    kputs("Setting syscall...");
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
    kputs("Done!\r\n");

    kputs("Entering user mode...\r\n\r\n");
    

    
    
    while (1);
}